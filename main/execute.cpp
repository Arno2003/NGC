#include <iostream>
#include <cstdlib> // For system()
#include <string>
using namespace std;

#include <filesystem>

string getFileNameWithoutExtension(const string& path) {
    std::filesystem::path filePath(path);
    return filePath.stem().string();
}

void bzip2Comp(string str){ //Tested OK - successfully
    string fileName = str;
    string file = getFileNameWithoutExtension(fileName);
    string command = "../executables/bzip2 " + fileName + " -9 -k > ../dna/comp/" + file + ".bz2";
    cout << command << endl;
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}

void paq8Comp(string str){  //Tested Ok - successfully
    string fileName = str;
    string command = "../executables/paq8px " + fileName + " ../dna/comp/ -8";
    cout << command << endl;
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}

void zip7Comp(string str){  // Tested OK - Successfully compressed
    string fileName = str;
    string file = getFileNameWithoutExtension(fileName);
    string command = "7z a ../dna/comp/" + file + ".7z " + fileName + " -m0=PPMD";
    cout << command << endl;
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}

void bscComp(string str){  // Unable to test - BSC not running
    string fileName = str;
    string file = getFileNameWithoutExtension(fileName);
    string command = "../executables/bsc " + fileName + " ../dna/comp/" + file + ".bsc -ca -e2 -l";
    cout << command << endl;
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}


void compressSequence(std::string sequence) {
    int choice = 10;
    cout << "Choose the compression method: \n1 for 7zip\n2 for PAQ8\n3 for BSC\n4 for GZIP\n5 for ZSTD\n6 for BZIP2\n7 for lpaq8\n8 for zpaq\n9 for Huffman\n0 to exit\n";
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "Compressing using 7zip..." << endl;
            zip7Comp(sequence); //Tested OK - Successfully compressed
            break;
        case 2:
            cout << "Compressing using PAQ8..." << endl;
            paq8Comp(sequence); //Tested OK - successfully
            break;
        case 3:
            cout << "Compressing using BSC..." << endl;
            bscComp(sequence);  // Unable to test - BSC not running
            break;
        case 4:
            cout << "Compressing using GZIP..." << endl;
            // Add GZIP compression logic here
            break;
        case 5:
            cout << "Compressing using ZSTD..." << endl;
            // Add ZSTD compression logic here
            break;
        case 6:
            cout << "Compressing using BZIP2..." << endl;
            bzip2Comp(sequence);    //Tested OK - successfully
            break;
        case 7:
            cout << "Compressing using lpaq8..." << endl;
            // Add lpaq8 compression logic here
            break;
        case 8:
            cout << "Compressing using zpaq..." << endl;
            // Add zpaq compression logic here
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
}

