#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <algorithm>
#include <unordered_map>

// Placeholder for the external function definition
void compressSequence(const std::string& filepath) {
    std::cout << "Compressing sequence: " << filepath << std::endl;
    // Example placeholder action
}

namespace fs = std::filesystem;

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

// Function to clean and normalize the sequence
std::string cleanSequence(const std::string& sequence) {
    std::string cleaned;
    for (char nucleotide : sequence) {
        nucleotide = std::toupper(static_cast<unsigned char>(nucleotide));
        if (nucleotide == 'A' || nucleotide == 'C' || nucleotide == 'G' ||
            nucleotide == 'T' || nucleotide == 'U') {
            cleaned += nucleotide;
        }
    }
    return cleaned;
}

// Convert nucleotide to 2-bit encoding
unsigned char nucleotideTo2Bit(char nucleotide) {
    switch (nucleotide) {
        case 'A': return 0b00;
        case 'C': return 0b01;
        case 'G': return 0b10;
        case 'T':
        case 'U': return 0b11;
        default: throw std::invalid_argument("Invalid nucleotide encountered.");
    }
}

// Encode sequence into 2-bit ASCII format
std::string encodeSequenceToASCII(const std::string& sequence) {
    std::string encoded;
    unsigned char currentByte = 0;
    int bitPosition = 0;

    for (char nucleotide : sequence) {
        unsigned char bits = nucleotideTo2Bit(nucleotide);
        currentByte |= (bits << (6 - bitPosition * 2));
        bitPosition++;
        if (bitPosition == 4) {
            encoded += static_cast<char>(currentByte);
            currentByte = 0;
            bitPosition = 0;
        }
    }
    if (bitPosition > 0) {
        encoded += static_cast<char>(currentByte);
    }
    return encoded;
}

// Read and parse sequence from file based on file type
std::string readSequenceFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string fileType = detectFileType(filename);
    std::string line, rawSequence;
    bool skipNextLine = false;  // For fastq '+' logic

    while (std::getline(infile, line)) {
        if (skipNextLine) {
            // Skip this line and reset the flag
            skipNextLine = false;
            continue;
        }

        if (line.empty()) {
            continue;
        }

        // FASTQ rules
        if (fileType == "fastq") {
            // Skip lines starting with '@'
            if (line[0] == '@') {
                continue;
            }
            // Skip line starting with '+' AND skip next line
            if (line[0] == '+') {
                skipNextLine = true;
                continue;
            }
        }
        // FASTA rule: skip lines starting with '>'
        else if (fileType == "fasta") {
            if (line[0] == '>') {
                continue;
            }
        }
        // RAW: no skipping

        // Validate characters (optional, can comment out if undesired)
        for (char c : line) {
            if (!std::isprint(static_cast<unsigned char>(c)) ||
                std::isspace(static_cast<unsigned char>(c))) {
                throw std::runtime_error("Invalid character found in sequence.");
            }
        }
        rawSequence += line;
    }

    if (rawSequence.empty()) {
        throw std::runtime_error("No valid sequence data found in file.");
    }

    infile.close();
    std::cout << "Sequence read successfully." << std::endl;
    std::cout << "Sequence length: " << rawSequence.size() << std::endl;
    std::cout << "RAW sequence : " << rawSequence << std::endl;
    return rawSequence;
}

// Main normalization function
void normalize(int argc, char* argv[]) {
    try {
        std::string inputFilePath = argv[1];
        if (!fs::exists(inputFilePath)) {
            throw std::runtime_error("Input file does not exist: " + inputFilePath);
        }

        std::cout << "Reading sequence from file..." << std::endl;
        std::string rawSequence = readSequenceFromFile(inputFilePath);

        std::cout << "Cleaning and normalizing sequence..." << std::endl;
        std::string cleanedSequence = cleanSequence(rawSequence);

        std::cout << "Encoding sequence into 2-bit ASCII format..." << std::endl;
        std::string encodedSequence = encodeSequenceToASCII(cleanedSequence);

        // Output directory "dna/norm"
        fs::path outputDir = "dna/norm";
        if (!fs::exists(outputDir)) {
            fs::create_directories(outputDir);
        }

        // Generate output file name
        fs::path inputFileName = fs::path(inputFilePath).stem();
        fs::path outputFilePath = outputDir / (inputFileName.string() + "_raw.txt");
        std::cout << "Saving encoded sequence to file: " << outputFilePath << std::endl;

        std::ofstream outfile(outputFilePath, std::ios::binary);
        if (!outfile.is_open()) {
            throw std::runtime_error("Cannot open output file for writing.");
        }
        outfile.write(encodedSequence.c_str(), encodedSequence.size());
        outfile.close();

        std::cout << "Compressing encoded sequence..." << std::endl;
        compressSequence(outputFilePath.string());

        std::cout << "Process completed successfully!" << std::endl;
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument error: " << ia.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}