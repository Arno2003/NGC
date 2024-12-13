#include <iostream>
using namespace std;
extern void decompressSequence(string);

int main(int argc, char* argv[]){
    string str = argv[1];
    cout<<str<<endl;
    decompressSequence(str);
    return 0;
}
