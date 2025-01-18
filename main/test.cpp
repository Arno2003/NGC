#include <iostream>
#include <cstring>
#include <string>
#include "defs.h"
using namespace std;

extern void decompressSequence(string);
extern void compressSequence(string);
extern void normalize(int argc, char* argv[]);
//extern void denormalize(int argc, char* argv[]);


int main(int argc, char* argv[]){
    string str = argv[1];
    cout << str << endl;
    string file = getFileNameWithoutExtension(str);
    char arg0[] = "_";
    char* arg1 = new char[str.size() + 1];
    strcpy(arg1, str.c_str());
    cout << "ARG1: " << arg1 << endl;
    char arg2[] = "_";
    char arg3[] = "1";
    cout <<"Checkpoint 1" << endl;
    char* argv2[] = {arg0, arg1, arg2, arg3};
    normalize(4, argv2);
    string inputFilePath = "../dna/norm/" + file + ".bin";
    cout << "Checkpoint2" << inputFilePath << endl;
    compressSequence(inputFilePath);
    decompressSequence(inputFilePath);
    //const char* argv3[] = {inputFilePath.c_str(), argv[2]};
    //denormalize(3, argv3);
    return 0;
}