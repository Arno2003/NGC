#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <filesystem>
#include <fstream> // Added to support std::ifstream
#include <stdbool.h>
#include <unistd.h>
#include "cpuUsage.h"

inline std::string getFileNameWithoutExtension(const std::string &path)
{
    std::filesystem::path filePath(path);
    return filePath.stem().string();
}

inline std::string getDirectoryName(const std::string &path)
{
    std::filesystem::path filePath(path);
    return filePath.parent_path().string();
}

inline std::string getFileExtension(const std::string &path)
{
    std::filesystem::path filePath(path);
    return filePath.extension().string();
}

// Function to detect file type based on extension
inline std::string detectFileType(const std::string &filename)
{
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos)
        return "raw";
    std::string ext = filename.substr(dotPos + 1);
    // Basic checks for known extensions
    if (ext == "fasta" || ext == "fa")
        return "fasta";
    if (ext == "fastq" || ext == "fq")
        return "fastq";
    return "raw";
}

inline void get_memory_usage(int *total, int *free)
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file))
    {
        if (sscanf(buffer, "MemTotal: %d kB", total) == 1 ||
            sscanf(buffer, "MemFree: %d kB", free) == 1)
        {
            // Do nothing, just parsing
        }
    }

    fclose(file);
}

inline void *get_pid_cpu_usage(void *arg)
{
    int *i = (int *)arg;
    sleep(1);
    std::string process = "";
    switch (*i)
    {
    case 1:
        process = "7z";
        break;
    case 2:
        process = "paq8px";
        break;
    case 3:
        process = "bsc";
        break;
    case 4:
        process = "gzip";
        break;
    case 5:
        process = "zstd";
        break;
    case 6:
        process = "bzip2";
        break;
    case 7:
        process = "lpaq8";
        break;
    case 8:
        process = "zpaq";
        break;
    case 9:
        process = "Huffman";
        break;
    case 10:
        process = "cmix";
        break;
    default:
        process = "unknown";
        break;
    }
    std::string command = "pgrep " + process + " > /tmp/pid.txt";
    int res = system(command.c_str());

    int pid;
    std::ifstream pidFile("/tmp/pid.txt");
    if (pidFile.is_open())
    {
        while (pidFile >> pid)
        {
            std::cout << "PID of " << process << ": " << pid << std::endl;
        }
        pidFile.close();
    }
    else
    {
        std::cerr << "Unable to open file to read PID." << std::endl;
    }
    std::cout << "\n\n\n: " << pid << "\n\n\n";
    get_cpu_usage(&pid);
    return nullptr;
}

#endif // DEFS_H