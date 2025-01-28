#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <algorithm>
#include <unordered_map>
#include <cstdint> // For fixed-width integer types
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "defs.h"

///////////////////////////////////////////////////////////
/////////////////////// RAM USAGE /////////////////////////

extern volatile bool keep_running;

int mem_total_norm, mem_free_beg_norm, mem_free_end_norm, mem_used_norm;
extern int cpu_avg, ram_avg;
int ram_total_norm;

extern void* get_cpu_usage(void* arg);

// void get_memory_usage(int* total, int* free) {
//     FILE* file = fopen("/proc/meminfo", "r");
//     if (!file) {
//         perror("fopen");
//         exit(EXIT_FAILURE);
//     }

//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), file)) {
//         if (sscanf(buffer, "MemTotal: %d kB", total) == 1 ||
//             sscanf(buffer, "MemFree: %d kB", free) == 1) {
//             // Do nothing, just parsing
//         }
//     }

//     fclose(file);
// }

//////////////////////////////////////////////////////////

// Function to clean and validate the sequence line
std::string cleanSequence(const std::string& line, int nucleotideType) {
    std::string cleaned;
    cleaned.reserve(line.length());
    int invalidCount = 0;
    for (char nucleotide : line) {
        char upperNucleotide = std::toupper(static_cast<unsigned char>(nucleotide));
        bool isValid = false;
        if (upperNucleotide == 'A' || upperNucleotide == 'C' || upperNucleotide == 'G') {
            isValid = true;
        }
        else if ((nucleotideType == 1 && upperNucleotide == 'T') || 
                 (nucleotideType == 2 && upperNucleotide == 'U')) {
            isValid = true;
        }

        if (isValid) {
            cleaned += upperNucleotide;
        }
        else {
            invalidCount++;
            // Optionally, log the invalid character
            // std::cerr << "Invalid character '" << nucleotide << "' skipped." << std::endl;
        }
    }
    if (invalidCount > 0) {
        std::cout << "Warning: " << invalidCount << " invalid characters skipped in a line." << std::endl;
    }
    return cleaned;
}

// Structure to hold encoded data and residual nucleotides
struct EncodedResult {
    std::string encodedBytes;
    uint8_t residualCount; // Number of residual nucleotides
    std::string residual;
};

// Function to convert a nucleotide to its 2-bit representation
unsigned char nucleotideTo2Bit(char nucleotide, int nucleotideType) {
    switch (nucleotide) {
        case 'A': return 0b00;
        case 'C': return 0b01;
        case 'G': return 0b10;
        case 'T':
            if (nucleotideType == 1) {
                return 0b11;
            }
            // Fall through for RNA
        case 'U':
            if (nucleotideType == 2) {
                return 0b11;
            }
            // Fall through for invalid cases
        default:
            throw std::invalid_argument("Invalid nucleotide: " + std::string(1, nucleotide));
    }
}

// Function to encode the raw sequence into 2-bit encoding
EncodedResult encodeSequenceTo2Bit(const std::string& sequence, int nucleotideType) {
    EncodedResult result;
    result.encodedBytes.reserve(sequence.length() / 4); // Reserve space for efficiency
    result.residualCount = 0;
    result.residual.reserve(3); // Maximum of 3 residual characters

    unsigned char currentByte = 0;
    int bitPosition = 0;

    for (size_t i = 0; i < sequence.length(); ++i) {
        char nucleotide = sequence[i];
        unsigned char bits = nucleotideTo2Bit(nucleotide, nucleotideType);
        currentByte |= (bits << (6 - bitPosition * 2));
        bitPosition++;

        if (bitPosition == 4) {
            result.encodedBytes += static_cast<char>(currentByte);
            currentByte = 0;
            bitPosition = 0;
        }
    }

    // Handle residual nucleotides if sequence length is not a multiple of 4
    if (bitPosition > 0) {
        size_t residualStart = sequence.length() - bitPosition;
        for (int j = 0; j < bitPosition; ++j) {
            result.residual += sequence[residualStart + j];
        }
        result.residualCount = static_cast<uint8_t>(bitPosition);
    }

    return result;
}

// Function to write the encoded sequence and residual to a file with metadata
void writeEncodedSequence(const EncodedResult& encodedResult, const std::string& outputPath) {
    std::ofstream outfile(outputPath, std::ios::binary);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open output file: " + outputPath);
    }

    // Write the encoded bytes
    outfile.write(encodedResult.encodedBytes.c_str(), encodedResult.encodedBytes.size());
    std::cout << "Encoded bytes written: " << encodedResult.encodedBytes.size() << " bytes" << std::endl;

    // Write residual nucleotides (if any)
    if (encodedResult.residualCount > 0) {
        outfile.write(encodedResult.residual.c_str(), encodedResult.residual.size());
        std::cout << "Residual nucleotides written: " << encodedResult.residual.size() << " characters" << std::endl;
    }

    // Write residual count as a single byte
    outfile.write(reinterpret_cast<const char*>(&encodedResult.residualCount), sizeof(encodedResult.residualCount));
    std::cout << "Residual nucleotide count written: " << static_cast<int>(encodedResult.residualCount) << std::endl;

    outfile.close();
    std::cout << "Encoded sequence written to " << outputPath << std::endl;
}

// Function to extract raw sequence from a file
std::string extractRawSequence(const std::string& filepath, int nucleotideType) {
    std::ifstream infile(filepath);
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filepath);
    }

    std::string fileType = detectFileType(filepath);
    std::string line;
    std::string rawSequence;
    bool skipNext = false; // Flag to skip the next line (used for FASTQ quality scores)
    int lineCount = 0;

    while (std::getline(infile, line)) {
        lineCount++;
        std::cout << "Processing line " << lineCount << std::endl;
        if (skipNext) {
            std::cout << "Skipping quality score line." << std::endl;
            skipNext = false;
            continue; // Skip the quality score line in FASTQ
        }

        if (fileType == "fastq") {
            if (line.empty()) {
                std::cout << "Empty line encountered. Skipping." << std::endl;
                continue; // Skip empty lines
            }
            char firstChar = line[0];
            if (firstChar == '@') {
                std::cout << "Header line detected. Skipping." << std::endl;
                continue; // Skip header lines
            }
            else if (firstChar == '+') {
                std::cout << "Separator line detected. Skipping this and the next line." << std::endl;
                skipNext = true;
                continue; // Skip separator and quality score lines
            }
            else {
                std::cout << "Sequence line detected." << std::endl;
                std::string cleaned = cleanSequence(line, nucleotideType);
                rawSequence += cleaned;
            }
        }
        else if (fileType == "fasta") {
            if (line.empty()) {
                std::cout << "Empty line encountered. Skipping." << std::endl;
                continue; // Skip empty lines
            }
            char firstChar = line[0];
            if (firstChar == '>') {
                std::cout << "Header line detected. Skipping." << std::endl;
                continue; // Skip header lines
            }
            else {
                std::cout << "Sequence line detected." << std::endl;
                std::string cleaned = cleanSequence(line, nucleotideType);
                rawSequence += cleaned;
            }
        }
        else { // raw
            std::cout << "Assuming raw sequence data." << std::endl;
            std::string cleaned = cleanSequence(line, nucleotideType);
            rawSequence += cleaned;
        }
    }

    infile.close();
    std::cout << "Raw sequence extraction completed." << std::endl;
    return rawSequence;
}

void normalize(int argc, char* argv[]) {

    ////////////////////////////////////////////////
    /////////// CPU AND MEM USAGE //////////////////

    pthread_t monitor_thread;
    int pid = (int)getpid();

    // Create a thread to monitor CPU usage
    pthread_create(&monitor_thread, NULL, get_cpu_usage, &pid);

    //////////////////////////////////////////
    /////////   MEM USAGE CALCULATE //////////

    get_memory_usage(&mem_total_norm, &mem_free_beg_norm);

    //////////////////////////////////////////

    if (argc < 4) { // Updated to require three arguments
        std::cerr << "Usage: normalize <input_file> <output_file> <1 (DNA) | 2 (RNA)>" << std::endl;
        return;
    }

    std::string inputFilePath = argv[1];

    std::string file = getFileNameWithoutExtension(inputFilePath);
    std::string outputFilePath = "../dna/norm/" + file + ".bin";

    std::cout << "checkpoint 1.1.1" << std::endl;
    std::cout << inputFilePath <<" "<< file <<" " << outputFilePath << std::endl;

    //std::string outputFilePath = argv[2];

    int nucleotideType = 0;

    try {
        nucleotideType = std::stoi(argv[3]);
        if (nucleotideType != 1 && nucleotideType != 2) {
            throw std::invalid_argument("Nucleotide type must be 1 (DNA) or 2 (RNA).");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid nucleotide type: " << e.what() << std::endl;
        return;
    }

    try {
        std::cout << "Starting raw sequence extraction from " << inputFilePath << std::endl;
        std::string rawSequence = extractRawSequence(inputFilePath, nucleotideType);
        std::cout << "Extracted Sequence Length: " << rawSequence.length() << " nucleotides" << std::endl;

        std::cout << "Starting 2-bit encoding of the sequence." << std::endl;
        EncodedResult encodedResult = encodeSequenceTo2Bit(rawSequence, nucleotideType);
        std::cout << "Encoded Bytes Length: " << encodedResult.encodedBytes.length() << " bytes" << std::endl;
        if (encodedResult.residualCount > 0) {
            std::cout << "Residual Nucleotides Length: " << static_cast<int>(encodedResult.residualCount) << " characters" << std::endl;
        }

        writeEncodedSequence(encodedResult, outputFilePath);
        std::cout << "Normalization process completed successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during normalization: " << e.what() << std::endl;
        return;
    }

    ////////////////////////////////////////////////
    /////////// CPU AND MEM USAGE //////////////////
    keep_running = false;

    // Wait for the monitoring thread to finish
    pthread_join(monitor_thread, NULL);


    get_memory_usage(&mem_total_norm, &mem_free_end_norm);
    if(mem_free_beg_norm > mem_free_end_norm)
        mem_used_norm = mem_free_beg_norm - mem_free_end_norm;
    ram_total_norm = (int)(mem_total_norm/1000);
    if(ram_avg == 0) ram_avg = 1;
    std::cout << "Memory used: " << mem_used_norm << " kb out of " << mem_total_norm << " kb" << std::endl;
    std::cout << "CPU usage: " << cpu_avg << " %" << std::endl;
    std::cout << "RAM usage: " << (ram_avg * ram_total_norm / 100) << " mb out of " << ram_total_norm << " mb" << std::endl;

    ////////////////////////////////////////////////
}