#include <iostream>
#include <cstdlib> // For system()
#include <string>
#include <filesystem>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <fstream>
#include <chrono> // Added for time measurement

#include "defs.h"
using namespace std;

extern string originalFile;
///////////////////////////////////////////////////////////
/////////////////////// RAM USAGE /////////////////////////

extern volatile bool keep_running;

int mem_total_comp, mem_free_beg_comp, mem_free_end_comp, mem_used_comp;
extern int cpu_avg, ram_avg, num_cpus;
int ram_total_comp;

extern void *get_cpu_usage(void *arg);

//////////////////////////////////////////////////////////

void zpaqComp(string str)
{ // New zpaq compression method
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string filePath = getDirectoryName(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "cd " + filePath + " && ../../executables/zpaq add ../comp/" + file + ".zpaq " + file + "" + fileExtension + " -method 5  -threads " + to_string(num_cpus*2);
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "zpaq compression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing zpaq compression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in zpaqComp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in zpaqComp." << endl;
    }
}

void bzip2Comp(string str)
{ // Tested OK - successfully
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "../executables/bzip2 -c " + fileName + " -9 -k > ../dna/comp/" + file + ".bz2";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "Command executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in bzip2Comp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in bzip2Comp." << endl;
    }
}

void paq8Comp(string str)
{ // Tested OK - successfully
    try
    {
        string fileName = str;
        string command = "../executables/paq8px " + fileName + " ../dna/comp/ -12";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "Command executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in paq8Comp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in paq8Comp." << endl;
    }
}

void zip7Comp(string str)
{ // Tested OK - Successfully compressed
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "7z a ../dna/comp/" + file + ".7z " + fileName + " -m0=lzma2 -mx=9 -m0:dict=8g -mmt=on -m0:fb=273";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "Command executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in zip7Comp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in zip7Comp." << endl;
    }
}

void bscComp(string str)
{ // Updated for maximum compression using BSC
    try
    {
        string fileName = str;                           // Full file path with name
        string file = getFileNameWithoutExtension(fileName); // File name without extension
        string filePath = getDirectoryName(fileName);    // Only path of file
        string fileExtension = getFileExtension(fileName); // e.g. ".txt" or ".bin"
        
        // Create a tar archive of the input file
        string command1 = "tar -cvf " + filePath + "/" + file + ".tar -C " + filePath + " " + file + fileExtension;
        
        // Run BSC with maximum compression options:
        // -e2: Best adaptive entropy encoding.
        // -b2047: Set block size to the maximum value.
        string command2 = "../executables/bsc e " + filePath + "/" + file + ".tar ../dna/comp/" + file + ".bsc -e2 -b2047";
        
        // Remove the temporary tar file after compression.
        string command3 = "rm " + filePath + "/" + file + ".tar";
        
        cout << command1 << endl;
        cout << command2 << endl;
        cout << command3 << endl;
        
        int retCode1 = system(command1.c_str()); // Create tar file.
        int retCode2 = system(command2.c_str()); // Compress using BSC.
        int retCode3 = system(command3.c_str()); // Remove the tar file.
        
        if (retCode1 == 0 && retCode2 == 0 && retCode3 == 0)
        {
            cout << "BSC maximum compression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing BSC compression. Return codes: " << retCode1 << " : " << retCode2 << " : " << retCode3 << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in bscComp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in bscComp." << endl;
    }
}

void gzipComp(string str)
{ // Tested OK - Successfully compressed
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "gzip -9 -c " + fileName + " > ../dna/comp/" + file + ".gz";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "Command executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in gzipComp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in gzipComp." << endl;
    }
}

// New ZSTD compression method
void zstdComp(string str)
{
    try
    {
        string fileName = str;                               // Full input file path
        string file = getFileNameWithoutExtension(fileName); // File name without extension
        string outputPath = "../dna/comp/" + file + ".zst";    // Output file path

        // New command with ultra compression options:
        // --ultra: enable ultra compression (levels beyond 19, up to 22)
        // -22: highest compression level, -T0: use all available threads, -k: keep original file.
        string command = "../executables/zstd --ultra -22 -T0 -k " + fileName + " -o " + outputPath;
        cout << command << endl;

        int retCode = system(command.c_str()); // Execute the command

        if (retCode == 0)
        {
            cout << "ZSTD ultra compression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing ZSTD ultra compression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in zstdComp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in zstdComp." << endl;
    }
}


void cmixComp(string str)
{
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        // Set the output file path for compressed file
        string output = "../dna/comp/" + file + ".cmix";
        // Basic compression command (without dictionary)
        string command = "../executables/cmix -c " + fileName + " " + output;
        cout << command << endl;
        int retCode = system(command.c_str());
        if (retCode == 0)
        {
            cout << "CMIX compression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing CMIX compression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in cmixComp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in cmixComp." << endl;
    }
}

void showCompressionRatio(int i, string str)
{
    string rawFile = originalFile;
    string fileName = str;
    string extension = getFileExtension(fileName);
    string file = getFileNameWithoutExtension(fileName);
    string compressedFile = "../dna/comp/" + file + "";
    string fileExtension;
    switch (i)
    {
    case 1:
        fileExtension = ".7z";
        break;
    case 2:
        fileExtension = extension + ".paq8px208fix1";
        break;
    case 3:
        fileExtension = ".bsc";
        break;
    case 4:
        fileExtension = ".gz";
        break;
    case 5:
        fileExtension = ".zst";
        break;
    case 6:
        fileExtension = ".bz2";
        break;
    case 7:
        fileExtension = ".zpaq";
        break;
    case 8:
        fileExtension = ".cmix";
    case 0:
        return;
    default:
        break;
    }
    compressedFile += fileExtension;

    // Calculate and print the compression ratio using std::filesystem
    try
    {
        double rawSize = std::filesystem::file_size(rawFile);
        double compSize = std::filesystem::file_size(compressedFile);
        double ratio = (rawSize / compSize);
        cout << "compressed size: "<< compSize << endl;
        cout << "raw size: "<< rawSize << endl;
        cout << "Compression ratio: " << ratio << endl;
    }
    catch (std::filesystem::filesystem_error &e)
    {
        cerr << "Filesystem error in showCompressionRatio: " << e.what() << endl;
    }
}

void compressSequence(std::string sequence)
{
    try
    {
        int choice = 10;
        while (choice)
        {
            cout << "Choose the compression method: \n1 for 7zip\n2 for PAQ8\n3 for BSC\n4 for GZIP\n5 for ZSTD\n6 for BZIP2\n7 for lpaq8\n8 for zpaq\n9 for Huffman\n10 for Cmix\n0 to exit\n";
            cin >> choice;

            // RAM AND CPU USAGE

            pthread_t monitor_thread_1;
            if (choice != 0)
            {
                ////////////////////////////////////////////////
                /////////// CPU AND MEM USAGE //////////////////

                int pid = (int)getpid();
                keep_running = true; // became false after normalization

                // Create a thread to monitor CPU usage
                // pthread_create(&monitor_thread, NULL, get_cpu_usage, &pid);
                pthread_create(&monitor_thread_1, NULL, get_pid_cpu_usage, &choice);

                //////////////////////////////////////////
                /////////   MEM USAGE CALCULATE //////////

                get_memory_usage(&mem_total_comp, &mem_free_beg_comp);
            }

            // Start timer before switch-case
            auto start = std::chrono::steady_clock::now();

            //////////////////////////////////////////

            switch (choice)
            {
            case 1:
                cout << "Compressing using 7zip..." << endl;
                zip7Comp(sequence); // Tested OK - Successfully compressed
                break;
            case 2:
                cout << "Compressing using PAQ8..." << endl;
                paq8Comp(sequence); // Tested OK - successfully
                break;
            case 3:
                cout << "Compressing using BSC..." << endl;
                bscComp(sequence); // Unable to test - BSC not running
                break;
            case 4:
                cout << "Compressing using GZIP..." << endl;
                gzipComp(sequence); // Tested OK - Successfully compressed
                break;
            case 5:
                cout << "Compressing using ZSTD..." << endl;
                zstdComp(sequence); // Added ZSTD compression logic
                break;
            case 6:
                cout << "Compressing using BZIP2..." << endl;
                bzip2Comp(sequence); // Tested OK - successfully
                break;
            case 7:
                cout << "Compressing using zpaq..." << endl;
                zpaqComp(sequence); // Call the new zpaqComp method
                break;
            case 8:
                cout << "Compressing using CMIX..." << endl;
                cmixComp(sequence); // Call the new cmixComp method
                break;
            case 0:
                cout << "Exiting..." << endl;
                return;
            default:
                cout << "Invalid choice. Exiting..." << endl;
                return;
            }

            // End timer after switch-case and print duration
            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            cout << "Compression time: " << elapsed / 1000 << " s" << endl;

            ////////////////////////////////////////////////
            /////////// CPU AND MEM USAGE //////////////////

            if (choice != 0)
            {
                keep_running = false;

                // Wait for the monitoring thread to finish
                pthread_join(monitor_thread_1, NULL);

                get_memory_usage(&mem_total_comp, &mem_free_end_comp);
                if (mem_free_beg_comp > mem_free_end_comp)
                    mem_used_comp = mem_free_beg_comp - mem_free_end_comp;
                ram_total_comp = (int)(mem_total_comp / 1000);
                if (ram_avg == 0)
                    ram_avg = 1;
                cout << "Memory used: " << mem_used_comp << " kb out of " << mem_total_comp << " kb" << endl;
                cout << "CPU usage: " << cpu_avg / num_cpus << " %" << endl;
                cout << "RAM usage: " << (ram_avg * ram_total_comp / 100) << " mb out of " << ram_total_comp << " mb" << endl;
            }
            ////////////////////////////////////////////////

            showCompressionRatio(choice, sequence);
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in compressSequence: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in compressSequence." << endl;
    }
}