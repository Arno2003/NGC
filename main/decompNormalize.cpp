#include <iostream>
#include <cstdlib> // For system()
#include <string>
#include <filesystem>
using namespace std;

string getFileNameWithoutExtension(const string& path) {
    std::filesystem::path filePath(path);
    return filePath.stem().string();
}

void zpaqDecomp(string str) { // Add ZPAQ Decompression Logic
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "../executables/zpaq x " + fileName + " -to main/data/decompressed_seq/";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "ZPAQ decompression executed successfully." << endl;
        } else {
            cerr << "Error executing ZPAQ decompression. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in zpaqDecomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in zpaqDecomp." << endl;
    }
}

void bzip2Decomp(string str) { // Tested OK - successfully
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "../executables/bzip2 -d " + fileName + " -9 -k > ../dna/comp/" + file + ".bz2";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "Command executed successfully." << endl;
        } else {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in bzip2Comp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in bzip2Comp." << endl;
    }
}

void paq8Decomp(string str) { // Tested OK - successfully
    try {
        string fileName = str;
        string command = "../executables/paq8px " + fileName + " ../dna/comp/ -8";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "Command executed successfully." << endl;
        } else {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in paq8Comp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in paq8Comp." << endl;
    }
}

void zip7Decomp(string str) { // Tested OK - Successfully compressed
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "7z a ../dna/comp/" + file + ".7z " + fileName + " -m0=PPMD";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "Command executed successfully." << endl;
        } else {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in zip7Comp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in zip7Comp." << endl;
    }
}

void bscDecomp(string str) { // Unable to test - BSC not running
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "../executables/bsc " + fileName + " ../dna/comp/" + file + ".bsc -ca -e2 -l";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "Command executed successfully." << endl;
        } else {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in bscComp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in bscComp." << endl;
    }
}

void gzipDecomp(string str) { // Tested OK - Successfully compressed
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "gzip -9 -c " + fileName + " > ../dna/comp/" + file + ".gz";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "Command executed successfully." << endl;
        } else {
            cerr << "Error executing command. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in gzipComp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in gzipComp." << endl;
    }
}

void decompressSequence(std::string sequence) {
    try {
        int choice = 10;
        cout << "Choose the compression method: \n1 for 7zip\n2 for PAQ8\n3 for BSC\n4 for GZIP\n5 for ZSTD\n6 for BZIP2\n7 for lpaq8\n8 for zpaq\n9 for Huffman\n0 to exit\n";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Compressing using 7zip..." << endl;
                zip7Decomp(sequence); // Tested OK - Successfully compressed
                break;
            case 2:
                cout << "Compressing using PAQ8..." << endl;
                paq8Decomp(sequence); // Tested OK - successfully
                break;
            case 3:
                cout << "Compressing using BSC..." << endl;
                bscDecomp(sequence); // Unable to test - BSC not running
                break;
            case 4:
                cout << "Compressing using GZIP..." << endl;
                gzipDecomp(sequence); // Tested OK - Successfully compressed
                break;
            case 5:
                cout << "Compressing using ZSTD..." << endl;
                // Add ZSTD compression logic here
                break;
            case 6:
                cout << "Compressing using BZIP2..." << endl;
                bzip2Decomp(sequence); // Tested OK - successfully
                break;
            case 7:
                cout << "Compressing using lpaq8..." << endl;
                // Add lpaq8 compression logic here
                break;
            case 8:
                cout << "Decompressing using ZPAQ..." << endl;
                zpaqDecomp(sequence);
                break;
            case 9:
                cout << "Compressing using Huffman..." << endl;
                // Add Huffman compression logic here
                break;
            case 0:
                cout << "Exiting..." << endl;
                return;
            default:
                cout << "Invalid choice. Exiting..." << endl;
                return;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in decompressSequence: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in decompressSequence." << endl;
    }
}