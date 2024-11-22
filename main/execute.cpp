#include <iostream>
#include <cstdlib> // For system()
#include <string>
using namespace std;


void compressSequence(std::string sequence) {
    int choice = 10;
    cout << "Choose the compression method: \n1 for 7zip\n2 for PAQ8\n3 for BSC\n4 for GZIP\n5 for ZSTD\n6 for BZIP2\n7 for lpaq8\n8 for zpaq\n9 for Huffman\n0 to exit\n";
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "Compressing using 7zip..." << endl;
            zip7Comp(sequence);
            break;
        case 2:
            cout << "Compressing using PAQ8..." << endl;
            paq8Comp(sequence);
            break;
        case 3:
            cout << "Compressing using BSC..." << endl;
            bscComp(sequence);
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
            bzip2Comp(sequence);
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

void bzip2Comp(string str){ //Not tested yet
    string fileName = str;
    string command = "./bzip2 " + fileName + "-9 -k";
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}

void paq8Comp(string str){  //Not tested yet
    string fileName = str;
    string command = "./paq8px -8 " + fileName + "compress/";
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}

void zip7Comp(string str){  // incomplete function.
    string fileName = str;
    string command = "7z -8 " + fileName + "compress/";
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}

void bscComp(string str){  // incomplete function.
    string fileName = str;
    string command = "./bsc -8 " + fileName + "compress/";
    int retCode = system(command.c_str()); //execute command

    if(retCode == 0){
        cout << "Command executed successfully.." << endl;
    } 
    else{
        cerr << "Error executing command.. Return code: " << retCode << endl;
    }
}