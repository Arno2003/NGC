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

///////////////////////////////////////////////////////////
/////////////////////// RAM USAGE /////////////////////////

extern volatile bool keep_running;

int mem_total_decomp, mem_free_beg_decomp, mem_free_end_decomp, mem_used_decomp;
extern int cpu_avg, ram_avg, num_cpus;
int ram_total_decomp;

extern void *get_cpu_usage(void *arg);

//////////////////////////////////////////////////////////

void zpaqDecomp(string str)
{ // Implemented ZPAQ decompression - test not complete
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "../executables/zpaq x ../dna/comp/" + file + ".zpaq -to ../dna/decomp/";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "ZPAQ decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing ZPAQ decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in zpaqDecomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in zpaqDecomp." << endl;
    }
}

void bzip2Decomp(string str)
{ // Implemented BZIP2 decompression - test ok success
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "../executables/bzip2 -d -k ../dna/comp/" + file + ".bz2 -c > ../dna/decomp/" + file + "" + fileExtension;
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "BZIP2 decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing BZIP2 decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in bzip2Decomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in bzip2Decomp." << endl;
    }
}

void paq8Decomp(string str)
{ // Implemented PAQ8 decompression - test ok success
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "../executables/paq8px -d ../dna/comp/" + file + "" + fileExtension + ".paq8px208fix1 ../dna/decomp/";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "PAQ8 decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing PAQ8 decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in paq8Decomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in paq8Decomp." << endl;
    }
}

void zip7Decomp(string str)
{ // Implemented 7zip decompression - test OK success
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "7z e ../dna/comp/" + file + ".7z -o../dna/decomp/ -y";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "7zip decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing 7zip decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in zip7Decomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in zip7Decomp." << endl;
    }
}

void bscDecomp(string str)
{ // Implemented BSC decompression - test ok success
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        // string fileExtension = getFileExtension(fileName);
        string command1 = "../executables/bsc d ../dna/comp/" + file + ".bsc ../dna/decomp/" + file + ".tar -e2";
        string command2 = "tar -xvf ../dna/decomp/" + file + ".tar -C ../dna/decomp/";
        string command3 = "rm ../dna/decomp/" + file + ".tar";
        cout << command1 << endl
             << command2 << endl
             << command3 << endl;
        int retCode1 = system(command1.c_str()); // execute first command
        int retCode2 = system(command2.c_str()); // execute second command
        int retCode3 = system(command3.c_str()); // remove extra .tar file

        if (retCode1 == 0 && retCode2 == 0 && retCode3 == 0)
        {
            cout << "BSC decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing BSC decompression. Return codes: " << retCode1 << " : " << retCode2 << " : " << retCode3 << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in bscDecomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in bscDecomp." << endl;
    }
}

void gzipDecomp(string str)
{ // Implemented GZIP decompression - test ok success
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "gzip -d -k ../dna/comp/" + file + ".gz -c > ../dna/decomp/" + file + "" + fileExtension;
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "GZIP decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing GZIP decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in gzipDecomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in gzipDecomp." << endl;
    }
}

// New ZSTD decompression method
void zstdDecomp(string str)
{ // yet to implement - not complete // if ok modify comment
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "../executables/zstd -d -o ../dna/decomp/" + file + "" + fileExtension + " ../dna/comp/" + file + ".zst";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0)
        {
            cout << "ZSTD decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing ZSTD decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in zstdDecomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in zstdDecomp." << endl;
    }
}

void huffmanDecomp(string str)
{
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        // Assuming huffman_decode reads the input file and outputs the decompressed data to stdout
        string command = "../executables/huffman_decode " + fileName + ".huffman";
        cout << command << endl;
        int retCode = system(command.c_str());
        if (retCode == 0)
        {
            cout << "Huffman decompression executed successfully." << endl;
            std::string outputFile = file; // assuming the decompressed file is named after the base file name
            std::string moveCommand = "mv " + outputFile + " ../dna/decomp/";
            std::cout << "Executing move command: " << moveCommand << std::endl;
            int moveCode = system(moveCommand.c_str());
            if (moveCode == 0)
            {
                std::cout << "File moved to dna/decomp successfully." << std::endl;
            }
            else
            {
                std::cerr << "Error moving file. Return code: " << moveCode << std::endl;
            }
        }
        else
        {
            cerr << "Error executing Huffman decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in huffmanDecomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in huffmanDecomp." << endl;
    }
}

void cmixDecomp(string str)
{
    try
    {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        // Set the output file path for the decompressed file
        string output = "../dna/decomp/" + file + ".bin";
        // Basic decompression command (without dictionary)
        string command = "../executables/cmix -d " + fileName + " " + output;
        cout << command << endl;
        int retCode = system(command.c_str());
        if (retCode == 0)
        {
            cout << "CMIX decompression executed successfully." << endl;
        }
        else
        {
            cerr << "Error executing CMIX decompression. Return code: " << retCode << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in cmixDecomp: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in cmixDecomp." << endl;
    }
}

void decompressSequence(std::string sequence)
{
    try
    {
        int choice = 10;
        while (choice)
        {
            cout << "Choose the decompression method: \n"
                 << "1 for 7zip\n"
                 << "2 for PAQ8\n"
                 << "3 for BSC\n"
                 << "4 for GZIP\n"
                 << "5 for ZSTD\n"
                 << "6 for BZIP2\n"
                 << "7 for zpaq\n"
                 << "8 for Cmix\n"
                 << "0 to exit\n";
            cin >> choice;

            // RAM AND CPU USAGE

            pthread_t monitor_thread_2;
            if (choice != 0)
            {
                ////////////////////////////////////////////////
                /////////// CPU AND MEM USAGE //////////////////

                int pid = (int)getpid();
                keep_running = true; // became false after normalization

                // Create a thread to monitor CPU usage
                // pthread_create(&monitor_thread, NULL, get_cpu_usage, &pid);
                pthread_create(&monitor_thread_2, NULL, get_pid_cpu_usage, &choice);

                //////////////////////////////////////////
                /////////   MEM USAGE CALCULATE //////////

                get_memory_usage(&mem_total_decomp, &mem_free_beg_decomp);

                //////////////////////////////////////////
            }

            // Start timer before switch-case
            auto start = std::chrono::steady_clock::now();

            switch (choice)
            {
            case 1:
                cout << "Decompressing using 7zip..." << endl;
                zip7Decomp(sequence); // Implemented
                break;
            case 2:
                cout << "Decompressing using PAQ8..." << endl;
                paq8Decomp(sequence); // Implemented
                break;
            case 3:
                cout << "Decompressing using BSC..." << endl;
                bscDecomp(sequence); // Implemented
                break;
            case 4:
                cout << "Decompressing using GZIP..." << endl;
                gzipDecomp(sequence); // Implemented
                break;
            case 5:
                cout << "Decompressing using ZSTD..." << endl;
                zstdDecomp(sequence); // Implemented ZSTD decompression
                break;
            case 6:
                cout << "Decompressing using BZIP2..." << endl;
                bzip2Decomp(sequence); // Implemented
                break;
            case 7:
                cout << "Decompressing using ZPAQ..." << endl;
                zpaqDecomp(sequence); // Implemented
                break;
            case 8:
                cout << "Decompressing using CMIX..." << endl;
                cmixDecomp(sequence); // Implemented
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
            cout << "Decompression time: " << elapsed / 1000 << " s" << endl;

            ////////////////////////////////////////////////
            /////////// CPU AND MEM USAGE //////////////////

            if (choice != 0)
            {
                keep_running = false;

                // Wait for the monitoring thread to finish
                pthread_join(monitor_thread_2, NULL);

                get_memory_usage(&mem_total_decomp, &mem_free_end_decomp);
                if (mem_free_beg_decomp > mem_free_end_decomp)
                    mem_used_decomp = mem_free_beg_decomp - mem_free_end_decomp;
                ram_total_decomp = (int)(mem_total_decomp / 1000);
                if (ram_avg == 0)
                    ram_avg = 1;
                std::cout << "Memory used: " << mem_used_decomp << " kb out of " << mem_total_decomp << " kb" << std::endl;
                std::cout << "CPU usage: " << cpu_avg / num_cpus << " %" << std::endl;
                std::cout << "RAM usage: " << (ram_avg * ram_total_decomp / 100) << " mb out of " << ram_total_decomp << " mb" << std::endl;
            }
            ////////////////////////////////////////////////
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Exception in decompressSequence: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception in decompressSequence." << endl;
    }
}