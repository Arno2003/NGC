#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <cstdint> // For fixed-width integer types
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "defs.h"


///////////////////////////////////////////////////////////
/////////////////////// RAM USAGE /////////////////////////

extern volatile bool keep_running;

int mem_total_denorm, mem_free_beg_denorm, mem_free_end_denorm, mem_used_denorm;
extern int cpu_avg, ram_avg;
int ram_total_denorm;

extern void* get_cpu_usage(void* arg);

//////////////////////////////////////////////////////////

// Function to decode 2-bit representation to nucleotide
char twoBitToNucleotide(unsigned char bits, int nucleotideType) {
    switch (bits) {
        case 0b00: return 'A';
        case 0b01: return 'C';
        case 0b10: return 'G';
        case 0b11: 
            if (nucleotideType == 1) {
                return 'T';
            } 
            else if (nucleotideType == 2) {
                return 'U';
            }
            // Fall through for invalid cases
        default: throw std::invalid_argument("Invalid 2-bit value");
    }
}

// Function to denormalize the encoded file
std::string denormalizeSequence(const std::string& filepath, int nucleotideType) {
    std::ifstream infile(filepath, std::ios::binary);
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filepath);
    }

    // Determine the file size
    infile.seekg(0, std::ios::end);
    std::streampos fileSize = infile.tellg();
    infile.seekg(0, std::ios::beg);

    if (fileSize < 1) {
        throw std::runtime_error("Encoded file is empty.");
    }

    // Read residualCount (last byte)
    uint8_t residualCount = 0;
    infile.seekg(-1, std::ios::end);
    infile.read(reinterpret_cast<char*>(&residualCount), sizeof(residualCount));

    if (residualCount > 3) {
        throw std::runtime_error("Invalid residual nucleotide count: " + std::to_string(residualCount));
    }

    // Calculate the position where residual nucleotides start
    std::streampos residualPos = fileSize - static_cast<std::streamoff>(1) - static_cast<std::streamoff>(residualCount);
    if (residualPos < 0) {
        throw std::runtime_error("File too small to contain residual nucleotides as per residual count.");
    }

    // Read residual nucleotides
    std::string residual;
    if (residualCount > 0) {
        residual.resize(residualCount);
        infile.seekg(residualPos, std::ios::beg);
        infile.read(&residual[0], residualCount);
        if (infile.gcount() != residualCount) {
            throw std::runtime_error("Error reading residual nucleotides.");
        }
    }

    // Read encodedBytes
    size_t encodedBytesSize = residualPos;
    std::vector<char> encodedBytes(encodedBytesSize);
    if (encodedBytesSize > 0) {
        infile.seekg(0, std::ios::beg);
        infile.read(encodedBytes.data(), encodedBytesSize);
        if (static_cast<size_t>(infile.gcount()) != encodedBytesSize) {
            throw std::runtime_error("Error reading encoded bytes.");
        }
    }

    infile.close();

    // Decode each byte into four nucleotides
    std::string reconstructed;
    reconstructed.reserve(static_cast<size_t>(encodedBytesSize) * 4 + residual.size());

    for (char encodedByte : encodedBytes) {
        unsigned char ub = static_cast<unsigned char>(encodedByte);
        for (int shift = 6; shift >= 0; shift -= 2) {
            unsigned char bits = (ub >> shift) & 0b11;
            reconstructed += twoBitToNucleotide(bits, nucleotideType);
        }
    }

    // Append residual nucleotides as-is
    reconstructed += residual;

    return reconstructed;
}

void denormalize(int argc, char* argv[]) {

    
    if (argc < 4) { // Updated to require three arguments
        std::cerr << "Usage: denormalize <input_file> <output_file> <1 (DNA) | 2 (RNA)>" << std::endl;
        return;
    }

    ////////////////////////////////////////////////
    /////////// CPU AND MEM USAGE //////////////////

    pthread_t monitor_thread;
    int pid = (int)getpid();

    // Create a thread to monitor CPU usage
    pthread_create(&monitor_thread, NULL, get_cpu_usage, &pid);

    //////////////////////////////////////////
    /////////   MEM USAGE CALCULATE //////////

    get_memory_usage(&mem_total_denorm, &mem_free_beg_denorm);

    //////////////////////////////////////////

    std::string inputFilePath = argv[1];

    //std::string outputFilePath = argv[2];
    std::string file = getFileNameWithoutExtension(inputFilePath);
    std::string outputFilePath = "../dna/denorm/" + file + ".txt";

    {
        using std::cout;
        using std::endl;
        cout << "Input File Path: " << inputFilePath << endl;
        cout << "Output File Path: " << outputFilePath << endl;
    }

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
        std::cout << "Starting denormalization from " << inputFilePath << std::endl;
        std::string sequence = denormalizeSequence(inputFilePath, nucleotideType);
        std::cout << "Reconstructed Sequence Length: " << sequence.length() << " nucleotides" << std::endl;

        std::ofstream outfile(outputFilePath);
        if (!outfile.is_open()) {
            throw std::runtime_error("Unable to open output file: " + outputFilePath);
        }
        outfile << sequence;
        outfile.close();
        std::cout << "Denormalization completed successfully. Output written to " << outputFilePath << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during denormalization: " << e.what() << std::endl;
        return;
    }

    ////////////////////////////////////////////////
    /////////// CPU AND MEM USAGE //////////////////
    keep_running = false;

    // Wait for the monitoring thread to finish
    pthread_join(monitor_thread, NULL);


    get_memory_usage(&mem_total_denorm, &mem_free_end_denorm);
    if(mem_free_beg_denorm > mem_free_end_denorm)
        mem_used_denorm = mem_free_beg_denorm - mem_free_end_denorm;
    ram_total_denorm = (int)(mem_total_denorm/1000);
    if(ram_avg == 0) ram_avg = 1;
    std::cout << "Memory used: " << mem_used_denorm << " kb out of " << mem_total_denorm << " kb" << std::endl;
    std::cout << "CPU usage: " << cpu_avg << " %" << std::endl;
    std::cout << "RAM usage: " << (ram_avg * ram_total_denorm / 100) << " mb out of " << ram_total_denorm << " mb" << std::endl;

    ////////////////////////////////////////////////

    return;
}