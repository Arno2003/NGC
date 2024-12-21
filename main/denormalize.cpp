#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <chrono>
#include <sys/resource.h>

namespace fs = std::filesystem;

// Placeholder for the external decompressSequence function
void decompressSequence(const std::string& filepath) {
    std::cout << "Decompressing sequence: " << filepath << std::endl;
    // Example placeholder action
}

// Helper function to convert extended ASCII string to bytes
std::vector<unsigned char> decodeASCIIToBytes(const std::string& asciiStr) {
    std::vector<unsigned char> bytes;
    for (unsigned char c : asciiStr) {
        bytes.push_back(c);
    }
    return bytes;
}

// Helper function to detect file type based on extension
std::string detectFileType(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return "raw";
    std::string ext = filename.substr(dotPos + 1);
    if (ext == "fasta" || ext == "fa")  return "fasta";
    if (ext == "fastq" || ext == "fq")  return "fastq";
    return "raw";
}

// Decode 2-bit packed bytes into nucleotides (DNA or RNA)
std::string decodeBytesToSequence(const std::vector<unsigned char>& byte_data, bool isDNA) {
    std::string sequence;
    for (auto byte : byte_data) {
        for (int shift = 6; shift >= 0; shift -= 2) {
            unsigned char bits = (byte >> shift) & 0b11;
            switch (bits) {
                case 0b00: sequence += 'A'; break;
                case 0b01: sequence += 'C'; break;
                case 0b10: sequence += 'G'; break;
                case 0b11: sequence += (isDNA ? 'T' : 'U'); break;
                default:
                    throw std::runtime_error("Invalid 2-bit encoding.");
            }
        }
    }
    // Remove trailing 'A' if it was used as padding
    while (!sequence.empty() && sequence.back() == 'A') {
        sequence.pop_back();
    }
    return sequence;
}

// Denormalize function
void denormalize(int argc, char* argv[]) {
    auto startTime = std::chrono::steady_clock::now();

    try {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " <encoded_sequence_file_path> <flag>\n"
                      << "  flag: 1->DNA, 2->RNA\n";
            return;
        }

        std::string inputFilePath = argv[1];
        // Additional logic: use the flag to check if file is DNA or RNA
        int flag = std::stoi(argv[2]);
        bool isDNA = (flag == 1);

        if (!fs::exists(inputFilePath)) {
            throw std::runtime_error("Input file does not exist: " + inputFilePath);
        }

        // Detect file type
        std::string fileType = detectFileType(inputFilePath);

        // Step 1: Read file and skip lines for FASTQ/FASTA if needed
        std::ifstream infile(inputFilePath, std::ios::binary);
        if (!infile.is_open()) {
            throw std::runtime_error("Cannot open file: " + inputFilePath);
        }

        std::string line, ascii_sequence;
        bool skipNextLine = false; // For fastq '+' logic
        while (std::getline(infile, line)) {
            if (skipNextLine) {
                skipNextLine = false;
                continue;
            }
            if (line.empty()) {
                continue;
            }

            if (fileType == "fastq") {
                if (line[0] == '@') {
                    continue; // skip line
                }
                if (line[0] == '+') {
                    skipNextLine = true;
                    continue;
                }
            } else if (fileType == "fasta") {
                if (line[0] == '>') {
                    continue; // skip line
                }
            }
            // raw: no skipping

            ascii_sequence += line;
        }
        infile.close();

        // Step 2: Convert extended ASCII to bytes
        std::cout << "Decoding extended ASCII to bytes..." << std::endl;
        std::vector<unsigned char> byte_data = decodeASCIIToBytes(ascii_sequence);

        // Step 3: Decode bytes to nucleotide sequence (DNA or RNA)
        std::cout << "Decoding bytes to nucleotide sequence..." << std::endl;
        std::string nucleotide_sequence = decodeBytesToSequence(byte_data, isDNA);

        // Step 4: Decompress if needed
        std::cout << "Calling decompressSequence..." << std::endl;
        decompressSequence(inputFilePath);

        // Step 5: Ensure output directory exists
        fs::path outputDir = "../dna/denorm";
        if (!fs::exists(outputDir)) {
            fs::create_directories(outputDir);
        }

        // Step 6: Write the denormalized sequence to a file
        fs::path outputFilePath = outputDir / (fs::path(inputFilePath).stem().string() + "_denorm.txt");
        std::ofstream outfile(outputFilePath);
        if (!outfile.is_open()) {
            throw std::runtime_error("Cannot open output file for writing.");
        }
        outfile << nucleotide_sequence;
        outfile.close();

        std::cout << "Denormalized sequence written to: " << outputFilePath << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Report time & memory usage
    auto endTime = std::chrono::steady_clock::now();
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    std::cout << "Time taken: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
              << " ms" << std::endl;
    std::cout << "Max resident set size: " << usage.ru_maxrss << " KB" << std::endl;
}

