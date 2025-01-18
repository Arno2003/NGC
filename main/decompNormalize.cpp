#include <iostream>
#include <cstdlib> // For system()
#include <string>
#include <filesystem>
#include "defs.h"
using namespace std;

// string getFileNameWithoutExtension(const string& path) {
//     std::filesystem::path filePath(path);
//     return filePath.stem().string();
// }

// string getDirectoryName(const string& path) {
//     std::filesystem::path filePath(path);
//     return filePath.parent_path().string();
// }

void zpaqDecomp(string str) { // Implemented ZPAQ decompression - test not complete
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "../executables/zpaq x ../dna/comp/" + file + ".zpaq -to ../dna/decomp/";
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

void bzip2Decomp(string str) { // Implemented BZIP2 decompression - test ok success
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "../executables/bzip2 -d -k ../dna/comp/" + file + ".bz2 -c > ../dna/decomp/" + file + "" + fileExtension;
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "BZIP2 decompression executed successfully." << endl;
        } else {
            cerr << "Error executing BZIP2 decompression. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in bzip2Decomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in bzip2Decomp." << endl;
    }
}

void paq8Decomp(string str) { // Implemented PAQ8 decompression - test ok success
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "../executables/paq8px -d ../dna/comp/" + file + "" + fileExtension + ".paq8px208fix1 ../dna/decomp/";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "PAQ8 decompression executed successfully." << endl;
        } else {
            cerr << "Error executing PAQ8 decompression. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in paq8Decomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in paq8Decomp." << endl;
    }
}

void zip7Decomp(string str) { // Implemented 7zip decompression - test OK success
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string command = "7z e ../dna/comp/" + file + ".7z -o../dna/decomp/ -y";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "7zip decompression executed successfully." << endl;
        } else {
            cerr << "Error executing 7zip decompression. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in zip7Decomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in zip7Decomp." << endl;
    }
}

void bscDecomp(string str) { // Implemented BSC decompression - test ok success
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        //string fileExtension = getFileExtension(fileName);
        string command1 = "../executables/bsc d ../dna/comp/" + file + ".bsc ../dna/decomp/" + file + ".tar -e2";
        string command2 = "tar -xvf ../dna/decomp/" + file + ".tar -C ../dna/decomp/";
        string command3 = "rm ../dna/decomp/" + file + ".tar";
        cout << command1 << endl << command2 << endl << command3<< endl;
        int retCode1 = system(command1.c_str()); // execute first command
        int retCode2 = system(command2.c_str()); // execute second command
        int retCode3 = system(command3.c_str()); // remove extra .tar file

        if (retCode1 == 0 && retCode2 == 0 && retCode3 == 0) {
            cout << "BSC decompression executed successfully." << endl;
        } else {
            cerr << "Error executing BSC decompression. Return codes: " << retCode1 << " : " << retCode2 << " : " << retCode3 << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in bscDecomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in bscDecomp." << endl;
    }
}

void gzipDecomp(string str) { // Implemented GZIP decompression - test ok success
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "gzip -d -k ../dna/comp/" + file + ".gz -c > ../dna/decomp/" + file + "" + fileExtension;
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "GZIP decompression executed successfully." << endl;
        } else {
            cerr << "Error executing GZIP decompression. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in gzipDecomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in gzipDecomp." << endl;
    }
}

// New ZSTD decompression method
void zstdDecomp(string str) {   // yet to implement - not complete // if ok modify comment
    try {
        string fileName = str;
        string file = getFileNameWithoutExtension(fileName);
        string fileExtension = getFileExtension(fileName);
        string command = "../executables/zstd -d -o ../dna/decomp/" + file + "" + fileExtension + " ../dna/comp/" + file + ".zst";
        cout << command << endl;
        int retCode = system(command.c_str()); // execute command

        if (retCode == 0) {
            cout << "ZSTD decompression executed successfully." << endl;
        } else {
            cerr << "Error executing ZSTD decompression. Return code: " << retCode << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception in zstdDecomp: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in zstdDecomp." << endl;
    }
}

void decompressSequence(std::string sequence) {
    try {
        int choice = 10;
        while(choice){
            cout << "Choose the decompression method: \n"
                 << "1 for 7zip\n"
                 << "2 for PAQ8\n"
                 << "3 for BSC\n"
                 << "4 for GZIP\n"
                 << "5 for ZSTD\n"
                 << "6 for BZIP2\n"
                 << "7 for lpaq8\n"
                 << "8 for zpaq\n"
                 << "9 for Huffman\n"
                 << "0 to exit\n";
            cin >> choice;
            switch (choice) {
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
                    cout << "Decompressing using lpaq8..." << endl;
                    // Add lpaq8 decompression logic here
                    cout << "lpaq8 decompression not implemented yet." << endl;
                    break;
                case 8:
                    cout << "Decompressing using ZPAQ..." << endl;
                    zpaqDecomp(sequence); // Implemented
                    break;
                case 9:
                    cout << "Decompressing using Huffman..." << endl;
                    // Add Huffman decompression logic here
                    cout << "Huffman decompression not implemented yet." << endl;
                    break;
                case 0:
                    cout << "Exiting..." << endl;
                    return;
                default:
                    cout << "Invalid choice. Exiting..." << endl;
                    return;
            }
        }
        
    } catch (const std::exception& e) {
        cerr << "Exception in decompressSequence: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception in decompressSequence." << endl;
    }
}
