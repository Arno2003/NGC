#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <filesystem>
#include <exception>

namespace fs = std::filesystem;

// Function to apply general-purpose decoder (e.g., decompression)
void applyGeneralPurposeDecoder(const std::string& filepath) {
    // Placeholder for actual decompression logic
    std::cout << "Applying general-purpose decoder to " << filepath << "..." << std::endl;
    // Implement decompression logic here or call external tool
    // Example: system(("gzip -d " + filepath).c_str());
}

// Function to decode 2-bit ASCII format back to nucleotide sequence
std::string decodeASCIItoSequence(const std::string& encodedData) {
    std::string sequence;
    for (unsigned char c : encodedData) {
        // Extract each 2-bit chunk
        for (int i = 6; i >= 0; i -= 2) {
            unsigned char bits = (c >> i) & 0b11;
            char nucleotide;
            switch (bits) {
                case 0b00: nucleotide = 'A'; break;
                case 0b01: nucleotide = 'C'; break;
                case 0b10: nucleotide = 'G'; break;
                case 0b11: nucleotide = 'T'; break;
                default: throw std::runtime_error("Error: Invalid bits encountered.");
            }
            sequence += nucleotide;
        }
    }
    return sequence;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <compressed_file_path>" << std::endl;
        return 1;
    }

    try {
        // Step 1: Apply general-purpose decoder
        std::string compressedFilePath = argv[1];
        applyGeneralPurposeDecoder(compressedFilePath);

        // Assume decompression removes compression extension
        fs::path decompressedFilePath = fs::path(compressedFilePath).replace_extension("");

        // Step 2: Read encoded data from file
        std::cout << "Reading encoded data from file..." << std::endl;
        std::ifstream infile(decompressedFilePath, std::ios::binary);
        if (!infile.is_open()) {
            throw std::runtime_error("Error: Cannot open file " + decompressedFilePath.string());
        }
        std::string encodedData((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        infile.close();

        // Step 3: Decode ASCII to nucleotide sequence
        std::cout << "Decoding encoded data..." << std::endl;
        std::string sequence = decodeASCIItoSequence(encodedData);

        // Step 4: Save sequence to file
        std::string outputFilePath = "decoded_sequence.txt";
        std::cout << "Saving decoded sequence to file: " << outputFilePath << std::endl;
        std::ofstream outfile(outputFilePath);
        if (!outfile.is_open()) {
            throw std::runtime_error("Error: Cannot open output file for writing.");
        }
        outfile << sequence;
        outfile.close();

        std::cout << "Process completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}