#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <cstdint> // For fixed-width integer types
#include "defs.h"

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

    std::string inputFilePath = argv[1];

    //std::string outputFilePath = argv[2];
    std::string file = getFileNameWithoutExtension(inputFilePath);
    std::string fileExtension = argv[2];
    std::string outputFilePath = "../dna/denorm/" + file + "" + fileExtension;

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

    return;
}