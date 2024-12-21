#include <iostream>
#include "defs.h"
using namespace std;
//extern void decompressSequence(string);
//extern void compressSequence(string);
extern void normalize(int argc, char* argv[]);
extern void denormalize(int argc, char* argv[]);


int main(int argc, char* argv[]){
    string str = argv[1];
    cout << str << endl;
    string file = getFileNameWithoutExtension(str);
    normalize(2, argv);
    string inputFilePath = "../dna/norm/" + file + "_raw.txt";
    //compressSequence(inputFilePath);
    //decompressSequence(inputFilePath);
    const char* argv2[] = {inputFilePath.c_str(), argv[2]};
    denormalize(3, argv);
    return 0;
}