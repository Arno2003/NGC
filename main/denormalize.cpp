#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>

// Function to decode 2-bit representation to nucleotide
char twoBitToNucleotide(unsigned char bits) {
    switch (bits) {
        case 0b00: return 'A';
        case 0b01: return 'C';
        case 0b10: return 'G';
        case 0b11: return 'T';
        default: throw std::invalid_argument("Invalid 2-bit value");
    }
}

// Function to denormalize the encoded file
std::string denormalizeSequence(const std::string& filepath) {
    std::ifstream infile(filepath, std::ios::binary);
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filepath);
    }

    std::vector<char> encodedBytes;
    std::string residual;
    std::string reconstructed;

    // Read encoded bytes up to newline
    char byte;
    while (infile.get(byte)) {
        if (byte == '\n') {
            break;
        }
        encodedBytes.push_back(byte);
    }

    // Read residual nucleotides
    std::getline(infile, residual);

    // Decode each byte into four nucleotides
    for (char encodedByte : encodedBytes) {
        unsigned char ub = static_cast<unsigned char>(encodedByte);
        for (int shift = 6; shift >= 0; shift -= 2) {
            unsigned char bits = (ub >> shift) & 0b11;
            reconstructed += twoBitToNucleotide(bits);
        }
    }

    // Append residual nucleotides
    reconstructed += residual;

    infile.close();
    return reconstructed;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: denormalize <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    try {
        std::cout << "Starting denormalization from " << inputFilePath << std::endl;
        std::string sequence = denormalizeSequence(inputFilePath);
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
        return 1;
    }

    return 0;
}