#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <algorithm>
#include <unordered_map>

// Placeholder for the external function definition
void compressSequence(const std::string& filepath) {
    // Implement compression logic or link with the appropriate library
    std::cout << "Compressing sequence: " << filepath << std::endl;
    // Example placeholder action
}

// Namespace alias
namespace fs = std::filesystem;

// Function to clean and normalize the sequence
std::string cleanSequence(const std::string& sequence) {
    std::string cleaned;
    for (char nucleotide : sequence) {
        nucleotide = std::toupper(static_cast<unsigned char>(nucleotide)); // Convert to uppercase
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
        default: throw std::invalid_argument("Invalid nucleotide encountered.");
    }
}

// Function to encode sequence into 2-bit format
std::string encodeSequenceToASCII(const std::string& sequence) {
    std::string encoded;
    unsigned char currentByte = 0;
    int bitPosition = 0;

    for (char nucleotide : sequence) {
        unsigned char bits = nucleotideTo2Bit(nucleotide);
        currentByte |= (bits << (6 - bitPosition * 2)); // Shift bits to correct position
        bitPosition++;

        if (bitPosition == 4) { // Every 4 nucleotides form 1 byte
            encoded += static_cast<char>(currentByte);
            currentByte = 0;
            bitPosition = 0;
        }
    }

    // Handle remaining nucleotides by padding with zeroes
    if (bitPosition > 0) {
        encoded += static_cast<char>(currentByte);
    }

    return encoded;
}

// Function to read sequence from file and validate
std::string readSequenceFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line, rawSequence;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        if (line[0] == '>' || line[0] == '@' || line[0] == '+') continue; // Skip headers
        if (line[0] == '+')  

        for (char c : line) {
            if (!std::isprint(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
                throw std::runtime_error("Invalid character found in sequence.");
            }
        }
        rawSequence += line;
    }

    if (rawSequence.empty()) {
        throw std::runtime_error("No valid sequence data found in file.");
    }

    infile.close();
    return rawSequence;
}
void normalize(int argc, char* argv[]) {
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " <sequence_file_path>" << std::endl;
    //     return;
    // }

    try {
        // Step 1: Input file
        std::string inputFilePath = argv[1];
        if (!fs::exists(inputFilePath)) {
            throw std::runtime_error("Input file does not exist: " + inputFilePath);
        }

        std::cout << "Reading sequence from file..." << std::endl;
        std::string rawSequence = readSequenceFromFile(inputFilePath);

        // Step 2: Clean sequence
        std::cout << "Cleaning and normalizing sequence..." << std::endl;
        std::string cleanedSequence = cleanSequence(rawSequence);

        // Step 3: Encode sequence
        std::cout << "Encoding sequence into 2-bit ASCII format..." << std::endl;
        std::string encodedSequence = encodeSequenceToASCII(cleanedSequence);

        // Step 4: Ensure output directory exists
        fs::path outputDir = "data/raw_seq";
        try {
            if (!fs::exists(outputDir)) {
                fs::create_directories(outputDir);
            }
        } catch (const fs::filesystem_error& e) {
            throw std::runtime_error("Unable to create directory " + outputDir.string() + ": " + e.what());
        }

        // Step 5: Save encoded sequence
        fs::path inputFileName = fs::path(inputFilePath).stem();
        fs::path outputFilePath = outputDir / (inputFileName.string() + "_raw");
        std::cout << "Saving encoded sequence to file: " << outputFilePath << std::endl;
        std::ofstream outfile(outputFilePath, std::ios::binary);
        if (!outfile.is_open()) {
            throw std::runtime_error("Cannot open output file for writing.");
        }
        outfile.write(encodedSequence.c_str(), encodedSequence.size());
        outfile.close();

        // Step 6: Call compressSequence
        std::cout << "Compressing encoded sequence..." << std::endl;
        compressSequence(outputFilePath.string());

        std::cout << "Process completed successfully!" << std::endl;
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument error: " << ia.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
