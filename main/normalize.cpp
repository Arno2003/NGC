#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <filesystem>
#include <exception>

namespace fs = std::filesystem;

// Function to read sequence from FASTA or FASTQ file
std::string readSequenceFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Error: Cannot open file " + filename);
    }

    std::string line;
    std::string rawSequence;
    bool isSequenceLine = false;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        if (line[0] == '>') {
            // FASTA header line
            continue;
        } else if (line[0] == '@') {
            // FASTQ header line
            isSequenceLine = true;
            continue;
        } else if (line[0] == '+') {
            // FASTQ separator line
            isSequenceLine = false;
            continue;
        } else {
            if (isSequenceLine || line[0] != '@') {
                rawSequence += line;
            }
        }
    }

    infile.close();
    return rawSequence;
}

// Function to clean and normalize the sequence
std::string cleanSequence(const std::string& sequence) {
    std::string cleaned;
    for (char nucleotide : sequence) {
        nucleotide = std::toupper(static_cast<unsigned char>(nucleotide)); // Convert to uppercase
        if (nucleotide == 'A' || nucleotide == 'C' || nucleotide == 'G' ||
            nucleotide == 'T' || nucleotide == 'U') {
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
        case 'T': return 0b11;
        case 'U': return 0b11;
        default: throw std::runtime_error("Error: Invalid nucleotide encountered.");
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

// Function to apply general-purpose encoder (e.g., compression)
void applyGeneralPurposeEncoder(const std::string& filepath) {
    // Placeholder for actual compression logic
    std::cout << "Applying general-purpose encoder to " << filepath << "..." << std::endl;
    // Implement compression logic here or call external tool
    // Example: system(("gzip " + filepath).c_str());
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sequence_file_path>" << std::endl;
        return 1;
    }

    try {
        // Step 1: Read sequence from file
        std::string inputFilePath = argv[1];
        std::cout << "Reading sequence from file..." << std::endl;
        std::string rawSequence = readSequenceFromFile(inputFilePath);

        // Step 2: Clean and normalize sequence
        std::cout << "Cleaning and normalizing sequence..." << std::endl;
        std::string cleanedSequence = cleanSequence(rawSequence);

        // Step 3: Encode sequence into 2-bit ASCII format
        std::cout << "Encoding sequence into 2-bit ASCII format..." << std::endl;
        std::string encodedSequence = encodeSequenceToASCII(cleanedSequence);

        // Step 4: Save encoded sequence to file
        fs::path outputFilePath = "encoded_sequence.bin";
        std::ofstream outfile(outputFilePath, std::ios::binary);
        if (!outfile.is_open()) {
            throw std::runtime_error("Error: Cannot open output file for writing.");
        }
        outfile.write(encodedSequence.data(), encodedSequence.size());
        outfile.close();

        // Step 5: Apply general-purpose encoder
        applyGeneralPurposeEncoder(outputFilePath.string());

        std::cout << "Process completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
