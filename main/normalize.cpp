#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <algorithm>
#include <unordered_map>

// Function to detect file type based on extension
std::string detectFileType(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return "raw";
    std::string ext = filename.substr(dotPos + 1);
    // Basic checks for known extensions
    if (ext == "fasta" || ext == "fa")    return "fasta";
    if (ext == "fastq" || ext == "fq")    return "fastq";
    return "raw";
}

// Function to clean and validate the sequence line
std::string cleanSequence(const std::string& line) {
    std::string cleaned;
    cleaned.reserve(line.length());
    for (char nucleotide : line) {
        char upperNucleotide = std::toupper(static_cast<unsigned char>(nucleotide));
        if (upperNucleotide == 'A' || upperNucleotide == 'C' ||
            upperNucleotide == 'G' || upperNucleotide == 'T' ||
            upperNucleotide == 'U') {
            cleaned += upperNucleotide;
        }
        // Optionally, handle or log invalid characters
    }
    return cleaned;
}

// Structure to hold encoded data and residual nucleotides
struct EncodedResult {
    std::string encodedBytes;
    std::string residual;
};

// Function to extract raw sequence from a file
std::string extractRawSequence(const std::string& filepath) {
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
                std::string cleaned = cleanSequence(line);
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
                std::string cleaned = cleanSequence(line);
                rawSequence += cleaned;
            }
        }
        else { // raw
            std::cout << "Assuming raw sequence data." << std::endl;
            std::string cleaned = cleanSequence(line);
            rawSequence += cleaned;
        }
    }

    infile.close();
    std::cout << "Raw sequence extraction completed." << std::endl;
    return rawSequence;
}

// Function to convert a nucleotide to its 2-bit representation
unsigned char nucleotideTo2Bit(char nucleotide) {
    switch (nucleotide) {
        case 'A': return 0b00;
        case 'C': return 0b01;
        case 'G': return 0b10;
        case 'T':
        case 'U': return 0b11;
        default:
            throw std::invalid_argument("Invalid nucleotide: " + std::string(1, nucleotide));
    }
}

// Function to encode the raw sequence into 2-bit encoding
EncodedResult encodeSequenceTo2Bit(const std::string& sequence) {
    EncodedResult result;
    result.encodedBytes.reserve(sequence.length() / 4); // Reserve space for efficiency
    std::string residual;
    residual.reserve(3); // Maximum of 3 residual characters

    unsigned char currentByte = 0;
    int bitPosition = 0;

    for (size_t i = 0; i < sequence.length(); ++i) {
        char nucleotide = sequence[i];
        unsigned char bits = nucleotideTo2Bit(nucleotide);
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
        // Instead of padding, we store the residual nucleotides separately
        for (int j = 0; j < bitPosition; ++j) {
            unsigned char bits = (currentByte >> (6 - j * 2)) & 0b11;
            switch (bits) {
                case 0b00: residual += 'A'; break;
                case 0b01: residual += 'C'; break;
                case 0b10: residual += 'G'; break;
                case 0b11: residual += 'T'; break;
                default: // This should never happen
                    throw std::runtime_error("Unexpected error during encoding.");
            }
        }
        result.residual = residual;
    }

    return result;
}

// Function to write the encoded sequence and residual to a file
void writeEncodedSequence(const EncodedResult& encodedResult, const std::string& outputPath) {
    std::ofstream outfile(outputPath, std::ios::binary);
    if (!outfile.is_open()) {
        throw std::runtime_error("Unable to open output file: " + outputPath);
    }

    // Write the encoded bytes
    outfile.write(encodedResult.encodedBytes.c_str(), encodedResult.encodedBytes.size());
    std::cout << "Encoded bytes written: " << encodedResult.encodedBytes.size() << " bytes" << std::endl;

    // If there are residual nucleotides, write a newline and then the residuals
    if (!encodedResult.residual.empty()) {
        std::cout << "Residual nucleotides detected: " << encodedResult.residual.size() << " characters" << std::endl;
        // Write a newline character (Unix-style)
        char newline = '\n';
        outfile.write(&newline, 1);
        // Write residual nucleotides as plain text
        outfile.write(encodedResult.residual.c_str(), encodedResult.residual.size());
        std::cout << "Residual nucleotides written to the next line." << std::endl;
    }

    outfile.close();
    std::cout << "Encoded sequence written to " << outputPath << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: normalize <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    try {
        std::cout << "Starting raw sequence extraction from " << inputFilePath << std::endl;
        std::string rawSequence = extractRawSequence(inputFilePath);
        std::cout << "Extracted Sequence Length: " << rawSequence.length() << " nucleotides" << std::endl;

        std::cout << "Starting 2-bit encoding of the sequence." << std::endl;
        EncodedResult encodedResult = encodeSequenceTo2Bit(rawSequence);
        std::cout << "Encoded Bytes Length: " << encodedResult.encodedBytes.length() << " bytes" << std::endl;
        if (!encodedResult.residual.empty()) {
            std::cout << "Residual Nucleotides Length: " << encodedResult.residual.length() << " characters" << std::endl;
        }

        writeEncodedSequence(encodedResult, outputFilePath);
        std::cout << "Normalization process completed successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during normalization: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}