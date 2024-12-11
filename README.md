# finalYear_journal

for temporary compilation
g++ -o test test.cpp execute.cpp
./test <file_path.txt>

7zip or LZMA - done
PAQ8 - done
BSC - done
GZIP -> mriganka
ZSTD -> arnab
BZIP2 - done
lpaq8, 
zpaq - (from LFQC) ->ananya
Huffman 

bzip2: http://sourceware.org/bzip2
LZMA: www.7-zip. org/sdk.html
Paq8: https://github.com/hxim/paq8px
lpaq8: (http://cs.fit.edu/m˜ mahoney/compression/#lpaq)
https://encode.su/threads/926-New-lpaq1-version
zpaq v7.02 (-method 5 -threads 4): http://mattmahoney.net/dc/zpaq.html - 

https://github.com/mariusmni/lfqc


make a build folder if not present:
mkdir build
cd build

compile using:
cmake ..
make



BSC-
To run BSC first .tar file is needed.
To convert a file in .tar use command
tar -cvf path_to_outputFile.tar path_to_inputFile.txt

then use BSC for compression
./bsc e path_to_outputFile.tar outputBscFile.bsc -e2

to decompress
./bsc d OutputBscFile.bsc tarOutputFile.tar

to decompress .tar to .txt 
tar -xvf ./tarOutputFile.tar 
Optional - change output tar directory using -C
tar -xvf ./tarOutputFile.tar -C ../dna/

to install gzip for system go to this website
https://ftp.gnu.org/gnu/gzip/
downlaod version gzip-1.13.tar.xz
then decompress 
xz -d file1.tar.xz
tar -xf file1.tar

then change diretory to ./gzip-1.13 and run the following commands.
./configure
make
sudo make install


