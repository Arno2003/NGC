#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>

extern void compressSequence(std::string sequence);

namespace fs = std::filesystem;

// Function to clean and normalize the sequence (step 1)
std::string cleanSequence(const std::string& sequence) {
    std::string cleaned;
    for (char nucleotide : sequence) {
        nucleotide = std::toupper(nucleotide); // Convert to uppercase
        if (nucleotide == 'A' || nucleotide == 'C' || nucleotide == 'G' || nucleotide == 'T' || nucleotide == 'U') {
            cleaned += nucleotide;
        }
    }
    return cleaned;
}

// Function to convert nucleotide to 2-bit encoding
unsigned char nucleotideTo2Bit(char nucleotide) {
    switch (nucleotide) {
        case 'A': return 0b00;
        case 'C': return 0b01;
        case 'G': return 0b10;
        case 'T': case 'U': return 0b11;
        default: throw std::runtime_error("Error: Invalid nucleotide encountered.");
    }
}

// Function to encode sequence into 2-bit format and combine into extended ASCII (step 2 & 3)
std::string encodeSequenceToASCII(const std::string& sequence) {
    std::string encoded;
    unsigned char currentByte = 0;
    int bitPosition = 0;

    for (char nucleotide : sequence) {
        unsigned char bits = nucleotideTo2Bit(nucleotide);
        currentByte |= (bits << (6 - bitPosition * 2)); // Shift bits to the correct position
        bitPosition++;

        if (bitPosition == 4) { // Every 4 nucleotides, store as a byte
            encoded += static_cast<char>(currentByte);
            currentByte = 0;
            bitPosition = 0;
        }
    }

    // Handle remaining nucleotides (pad with zeroes)
    if (bitPosition > 0) {
        encoded += static_cast<char>(currentByte);
    }

    return encoded;
}

// Function to validate file content and read sequence
std::string readSequenceFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Error: Cannot open file " + filename);
    }

    std::string line, rawSequence;
    while (std::getline(infile, line)) {
        // Skip headers or metadata lines in FASTA/FASTQ
        if (line.empty()) continue;
        if (line[0] == '>' || line[0] == '@' || line[0] == '+') {
            continue;
        }
        for (char c : line) {
            if (!std::isprint(c) || std::isspace(c)) {
                throw std::runtime_error("Error: Invalid character found in sequence.");
            }
        }
        rawSequence += line;
    }

    if (rawSequence.empty()) {
        throw std::runtime_error("Error: No valid sequence data found in file.");
    }

    infile.close();
    return rawSequence;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sequence_file_path>" << std::endl;
        return 1;
    }

    try {
        std::cout << "Reading sequence file..." << std::endl;
        std::string filename = argv[1]; // Get file path from command line
        std::string rawSequence = readSequenceFromFile(filename);

        std::cout << "Cleaning and normalizing sequence..." << std::endl;
        std::string cleanedSequence = cleanSequence(rawSequence);

        std::cout << "Encoding sequence to ASCII format..." << std::endl;
        std::string encodedSequence = encodeSequenceToASCII(cleanedSequence);

        // Ensure the output directory exists
        fs::path outputDir = "..//data//raw_seq";
        try {
            if (!fs::exists(outputDir)) {
                fs::create_directories(outputDir);
            }
        } catch (const std::exception& e) {
            throw std::runtime_error("Error: Failed to create output directory: " + std::string(e.what()));
        }

        // Generate output file name
        fs::path inputFilePath = filename;
        std::string outputFilename = inputFilePath.stem().string() + "_raw" + inputFilePath.extension().string();
        fs::path outputFilePath = outputDir / outputFilename;

        std::cout << "Saving encoded sequence to file..." << std::endl;
        std::ofstream outfile(outputFilePath, std::ios::binary);
        if (!outfile.is_open()) {
            throw std::runtime_error("Error: Cannot open output file for writing.");
        }
        outfile.write(encodedSequence.c_str(), encodedSequence.size());
        outfile.close();

        std::cout << "Encoded sequence successfully saved to " << outputFilePath << std::endl;
        
        // Compress the raw file according to user choice..
        compressSequence(outputFilename);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
