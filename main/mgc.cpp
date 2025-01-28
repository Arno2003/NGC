#include <iostream>
#include <cstring>
#include <string>
#include "defs.h"
using namespace std;

extern void decompressSequence(string);
extern void compressSequence(string);
extern void normalize(int argc, char* argv[]);
extern void denormalize(int argc, char* argv[]);


void clean_residuals(string str, int i){
    string file = getFileNameWithoutExtension(str);
    string cmd1 = "rm ../dna/norm/" + file + "*";
    //string cmd2 = "rm ../dna/comp/" + file + "*";
    string cmd3 = "rm ../dna/decomp/" + file + "*";
    //string cmd4 = "rm ../dna/denorm/" + file + "*";

    if (i == 2){
        int res1 = system(cmd1.c_str());
        int res3 = system(cmd3.c_str());
    }
    else if (i == 1){
        int res3 = system(cmd3.c_str());
    }
}

void proposed(int argc, char* argv[]){
    string str = argv[1];
    cout << str << endl;
    string file = getFileNameWithoutExtension(str);
    string fileExtension = getFileExtension(str);
    char arg0[] = "_";
    char* arg1 = new char[str.size() + 1];
    strcpy(arg1, str.c_str());
    cout << "ARG1: " << arg1 << endl;
    char arg2[] = "_";
    char arg3[] = "1";
    cout <<"Checkpoint 1" << endl;
    char* argv2[] = {arg0, arg1, arg2, arg3};
    ////////////////////////////////////////

    ////////////////////////////////////////
    normalize(4, argv2);
    string inputFilePath = "../dna/norm/" + file + ".bin";
    cout << "Checkpoint2" << inputFilePath << endl;
    compressSequence(inputFilePath);
    decompressSequence(inputFilePath);
    

    inputFilePath = "../dna/decomp/" + file + ".bin";
    char* arg4 = new char[inputFilePath.size() + 1];
    strcpy(arg4, inputFilePath.c_str());
    char* argv3[] = {arg0, arg4, arg2, arg3};
    ///////////////////////////////////////////

    ///////////////////////////////////////////
    denormalize(4, argv3);
}

void standard(int argc, char* argv[]){
    string str = argv[1];
    cout << str << endl;
    compressSequence(str);
    decompressSequence(str);
}

int main(int argc, char* argv[]){
    int i = 9;
    try{
        string str = argv[1];
        while(i){
            cout << "1 for standard\n2 for proposed\n0 to exit : \n\nEnter your choice: ";
            cin >> i;
            if ( i == 1) {
                standard(argc, argv);
            }
            else{
                proposed(argc, argv);
            }
            clean_residuals(str, i);
        }
    }
    catch (exception& e){
        cout << "Exception: " << e.what() << endl;
    }
    return 0;
}