#include <iostream>
using namespace std;
extern void decompressSequence(string);
extern void compressSequence(string);

int main(int argc, char* argv[]){
    string str = argv[1];
    cout<<str<<endl;
    compressSequence(str);
    decompressSequence(str);
    return 0;
}
