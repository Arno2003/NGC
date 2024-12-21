#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <algorithm>
#include <unordered_map>
#include <cctype>

// Placeholder for the external function definition;
// you can implement or link your compression logic here.
void compressSequence(const std::string& filepath) {
    std::cout << "Compressing sequence: " << filepath << std::endl;
    // Placeholder action
}

namespace fs = std::filesystem;

// Detect the file type based on extension
std::string detectFileType(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return "raw";
    std::string ext = filename.substr(dotPos + 1);
    if (ext == "fasta" || ext == "fa")    return "fasta";
    if (ext == "fastq" || ext == "fq")    return "fastq";
    return "raw";
}

// Clean and normalize the sequence (remove invalid characters, convert to uppercase)
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

// Convert nucleotide to 2-bit representation
unsigned char nucleotideTo2Bit(char nucleotide) {
    switch (nucleotide) {
        case 'A': return 0b00;
        case 'C': return 0b01;
        case 'G': return 0b10;
        case 'T':
        case 'U': return 0b11;
        default:
            throw std::invalid_argument("Invalid nucleotide encountered.");
    }
}

// Encode sequence into a series of 2-bit packed bytes
std::string encodeSequenceToASCII(const std::string& sequence) {
    std::string encoded;
    encoded.reserve((sequence.size() + 3) / 4); // approx. space for encoded bytes
    unsigned char currentByte = 0;
    int bitPosition = 0;

    for (char nucleotide : sequence) {
        unsigned char bits = nucleotideTo2Bit(nucleotide);
        // Shift bits into the correct position
        currentByte |= (bits << (6 - bitPosition * 2));
        bitPosition++;

        if (bitPosition == 4) {
            encoded.push_back(static_cast<char>(currentByte));
            currentByte = 0;
            bitPosition = 0;
        }
    }
    // Handle the last partial byte if any
    if (bitPosition > 0) {
        encoded.push_back(static_cast<char>(currentByte));
    }

    return encoded;
}

// Read and parse sequence from file, applying rules for FASTA, FASTQ, or RAW
std::string readSequenceFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string fileType = detectFileType(filename);
    std::string line;
    std::string rawSequence;
    bool skipNextLine = false;  // Used for FASTQ '+' lines

    while (std::getline(infile, line)) {
        if (skipNextLine) {
            skipNextLine = false;
            continue;
        }
        if (line.empty()) {
            continue;
        }
        // Handle FASTQ rules
        if (fileType == "fastq") {
            // Skip lines starting with '@'
            if (line[0] == '@') {
                continue;
            }
            // Skip line starting with '+' AND skip the next line as well
            if (line[0] == '+') {
                skipNextLine = true;
                continue;
            }
        }
        // Handle FASTA rule: skip lines starting with '>'
        else if (fileType == "fasta") {
            if (line[0] == '>') {
                continue;
            }
        }
        // For raw file type, process all lines directly

        // Remove or relax the whitespace checks to avoid errors
        // (previously enforced here).
        // Example: if you only want to skip non-printable but allow whitespace:
        for (char c : line) {
            if (!std::isprint(static_cast<unsigned char>(c))) {
                throw std::runtime_error("Invalid character found in sequence.");
            }
        }

        // Append this line's content to the raw sequence
        rawSequence += line;
    }

    if (rawSequence.empty()) {
        throw std::runtime_error("No valid sequence data found in file.");
    }

    infile.close();
    std::cout << "Sequence read successfully.\n"
              << "Sequence length: " << rawSequence.size() << "\n"
              << "RAW sequence : " << rawSequence << std::endl;
    return rawSequence;
}

// Main normalization function
void normalize(int argc, char* argv[]) {
    try {
        // In this example, argv[1] is assumed to be the input file path
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

        // Save the output in "dna/norm"
        fs::path outputDir = "dna/norm";
        if (!fs::exists(outputDir)) {
            fs::create_directories(outputDir);
        }

        // Create output file name
        fs::path inputFileName = fs::path(inputFilePath).stem();
        fs::path outputFilePath = outputDir / (inputFileName.string() + "_raw.txt");
        std::cout << "Saving encoded sequence to file: " << outputFilePath << std::endl;

        // Write the encoded data
        std::ofstream outfile(outputFilePath, std::ios::binary);
        if (!outfile.is_open()) {
            throw std::runtime_error("Cannot open output file for writing.");
        }
        outfile.write(encodedSequence.data(), encodedSequence.size());
        outfile.close();

        // Placeholder: call compressSequence (your compression logic)
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

// Example main function: invoke normalize with the path argument
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <sequence_file_path>" << std::endl;
        return 1;
    }
    normalize(argc, argv);
    return 0;
}