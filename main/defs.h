#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <filesystem>
#include <stdbool.h>
#include <unistd.h>

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

inline void get_memory_usage(int* total, int* free) {
    FILE* file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "MemTotal: %d kB", total) == 1 ||
            sscanf(buffer, "MemFree: %d kB", free) == 1) {
            // Do nothing, just parsing
        }
    }

    fclose(file);
}

#endif // DEFS_H