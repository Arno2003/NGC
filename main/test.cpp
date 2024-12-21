#include <iostream>
#include "defs.h"
using namespace std;
extern void decompressSequence(string);
extern void compressSequence(string);
extern void normalize(int argc, char* argv[]);


int main(int argc, char* argv[]){
    string str = argv[1];
    string file = getFileNameWithoutExtension(str);
    normalize(2, argv);
    string inputFilePath = "../dna/norm/" + file + ".txt";
    compressSequence(inputFilePath);
    decompressSequence(inputFilePath);
    return 0;
}