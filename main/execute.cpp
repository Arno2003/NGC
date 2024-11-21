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
            // Add 7zip compression logic here
            break;
        case 2:
            cout << "Compressing using PAQ8..." << endl;
            // Add PAQ8 compression logic here
            break;
        case 3:
            cout << "Compressing using BSC..." << endl;
            // Add BSC compression logic here
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
            // Add BZIP2 compression logic here
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

int main() {
    std::string fileName = "AbCd.co";  // File to process
    std::string outputFileName = "AbCd.gc";
    std::string command = "./geco -c " + fileName + " -o " + outputFileName + " -t22";

    std::cout << "Executing: " << command << std::endl;

    int retCode = system(command.c_str()); // Execute the command

    if (retCode == 0) {
        std::cout << "Command executed successfully." << std::endl;
    } else {
        std::cerr << "Error executing command. Return code: " << retCode << std::endl;
    }

    return 0;
}

void bzip2Comp(string str){
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