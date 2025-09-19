#g++ -o test test.cpp normalize.cpp compNormalize.cpp decompNormalize.cpp -std=c++17 -O3
#g++ -o test test.cpp normalize.cpp denormalize.cpp -std=c++17 -O3
g++ -o test ngc.cpp compNormalize.cpp decompNormalize.cpp normalize.cpp denormalize.cpp resourceUsage.c -std=c++17 -O3 -pthread