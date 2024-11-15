#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

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
        default: return 0b00; // Default case for safety
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

// Function to read input DNA/RNA sequence file and extract raw sequence
std::string readSequenceFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Error: Cannot open file " + filename);
    }

    std::string line, rawSequence;
    while (std::getline(infile, line)) {
        // Skip headers or metadata lines in FASTA/FASTQ
        if (line[0] != '>' && line[0] != '@' && line[0] != '+') {
            rawSequence += line;
        }
    }
    infile.close();
    return rawSequence;
}

int main() {
    try {
        std::string filename = "input.fasta"; // Change filename to test other formats like lfqc, fastq, etc.
        std::string rawSequence = readSequenceFromFile(filename);

        std::string cleanedSequence = cleanSequence(rawSequence);
        std::string encodedSequence = encodeSequenceToASCII(cleanedSequence);

        // Output the results
        std::cout << "Cleaned Sequence: " << cleanedSequence << std::endl;
        std::cout << "Encoded Sequence (Extended ASCII): " << encodedSequence << std::endl;

        // Optionally save encoded sequence to a file
        std::ofstream outfile("encoded_output.txt", std::ios::binary);
        outfile.write(encodedSequence.c_str(), encodedSequence.size());
        outfile.close();
        std::cout << "Encoded sequence saved to encoded_output.txt" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
