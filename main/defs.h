#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <filesystem>

inline std::string getFileNameWithoutExtension(const std::string& path) {
    std::filesystem::path filePath(path);
    return filePath.stem().string();
}

inline std::string getDirectoryName(const std::string& path) {
    std::filesystem::path filePath(path);
    return filePath.parent_path().string();
}

inline std::string getFileExtension(const std::string& path) {
    std::filesystem::path filePath(path);
    return filePath.extension().string();
}

// Function to detect file type based on extension
inline std::string detectFileType(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return "raw";
    std::string ext = filename.substr(dotPos + 1);
    // Basic checks for known extensions
    if (ext == "fasta" || ext == "fa")    return "fasta";
    if (ext == "fastq" || ext == "fq")    return "fastq";
    return "raw";
}

#endif // DEFS_H