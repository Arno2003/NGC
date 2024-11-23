#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <algorithm>
#include <vector>
#include <cctype>

namespace fs = std::filesystem;

// External function declaration (to be implemented later)
void decompressSequence(const std::string& filepath);

// Function to convert extended ASCII to bytes
std::vector<unsigned char> decodeASCIIToBytes(const std::string& ascii_sequence) {
    std::vector<unsigned char> byte_data;
    for(char c : ascii_sequence){
        byte_data.push_back(static_cast<unsigned char>(c));
    }
    return byte_data;
}

// Function to convert bytes to nucleotide sequence
std::string decodeBytesToSequence(const std::vector<unsigned char>& byte_data) {
    std::string sequence;
    for(unsigned char byte : byte_data){
        for(int shift = 6; shift >= 0; shift -= 2){
            unsigned char bits = (byte >> shift) & 0b11;
            switch(bits){
                case 0b00: sequence += 'A'; break;
                case 0b01: sequence += 'C'; break;
                case 0b10: sequence += 'G'; break;
                case 0b11: sequence += 'T'; break;
                default: throw std::runtime_error("Invalid 2-bit encoding.");
            }
        }
    }
    // Remove padding 'A's if any (assuming padding with 'A's which correspond to 0b00)
    while(!sequence.empty() && sequence.back() == 'A'){
        sequence.pop_back();
    }
    return sequence;
}

// Placeholder for the external decompressSequence function
void decompressSequence(const std::string& filepath) {
    // Implement decompression logic or link with the appropriate library
    std::cout << "Decompressing sequence: " << filepath << std::endl;
    // Example placeholder action
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <encoded_sequence_file_path>" << std::endl;
        return 1;
    }

    try {
        // Step 1: Input file
        std::string inputFilePath = argv[1];
        if (!fs::exists(inputFilePath)) {
            throw std::runtime_error("Input file does not exist: " + inputFilePath);
        }

        std::cout << "Reading encoded sequence from file..." << std::endl;
        std::ifstream infile(inputFilePath, std::ios::binary);
        if (!infile.is_open()) {
            throw std::runtime_error("Cannot open file: " + inputFilePath);
        }

        std::string ascii_sequence((std::istreambuf_iterator<char>(infile)),
                                   std::istreambuf_iterator<char>());
        infile.close();

        // Step 2: Decode extended ASCII to bytes
        std::cout << "Decoding extended ASCII to bytes..." << std::endl;
        std::vector<unsigned char> byte_data = decodeASCIIToBytes(ascii_sequence);

        // Step 3: Decode bytes to nucleotide sequence
        std::cout << "Decoding bytes to nucleotide sequence..." << std::endl;
        std::string nucleotide_sequence = decodeBytesToSequence(byte_data);

        // Step 4: Call decompressSequence (empty function for now)
        std::cout << "Calling decompressSequence (to be implemented)..." << std::endl;
        decompressSequence(inputFilePath);

        // Step 5: Ensure output directory exists
        fs::path outputDir = "data/decompressed_seq";
        try {
            if (!fs::exists(outputDir)) {
                fs::create_directories(outputDir);
            }
        } catch (const fs::filesystem_error& e) {
            throw std::runtime_error("Unable to create directory " + outputDir.string() + ": " + e.what());
        }

        // Step 6: Save reconstructed sequence
        fs::path inputFileName = fs::path(inputFilePath).stem();
        fs::path outputFilePath = outputDir / (inputFileName.string() + "_decompressed.txt");
        std::cout << "Saving reconstructed sequence to file: " << outputFilePath << std::endl;
        std::ofstream outfile(outputFilePath, std::ios::out);
        if (!outfile.is_open()) {
            throw std::runtime_error("Cannot open output file for writing.");
        }
        outfile << nucleotide_sequence;
        outfile.close();

        std::cout << "Decompression process completed successfully!" << std::endl;
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument error: " << ia.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
