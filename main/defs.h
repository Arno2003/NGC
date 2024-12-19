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

#endif // DEFS_H