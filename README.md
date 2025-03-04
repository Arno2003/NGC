#  MGC GENOME COMPRESSION
MGC maps ACGT sequences to respective 2-bit ASCII encoding values and then applies a general-purpose compression algorithm for further size reduction. The decompression process reverses this transformation to restore the original sequence.

## Supporting Platform

- Linux

## Tech Stack

- Backend: C++
- Frontend: NextJs


## Supported File Formats

- FASTA / Multi-FASTA
- FASTQ
- Raw sequence files

## Compression Methods and Decompression Method

The general-purpose compressors:

- 7-zip (https://www.7-zip.org)
- paq8 (https://github.com/hxim/paq8px)
- bsc (https://github.com/IlyaGrebnov/libbsc)
- gzip (https://www.gnu.org/software/gzip/)
- zstd (https://github.com/facebook/zstd.git)
- bzip2 (http://sourceware.org/bzip2)
- zpaq (http://mattmahoney.net/dc/zpaq.html)
- cmix (https://github.com/byronknoll/cmix)



## Requirements
1. Install 7zip using command
    ```sh
    $ sudo apt install 7zip
    ```
2. Install gzip using command
    ```sh
    $ sudo apt install gzip
    ```
3. Other general purpose executables come with main repository

4. GCC compiler version: 13 (works with >=9.0)

5. To compress DNA files place original files in dna/raw/ folder.

6. bzip2, 7zip and some other compressors requires large system memory (~30 GB RAM).


## Installation

1. Clone the repository:
    ```sh
   $ git clone https://github.com/Arno2003/MGC

   $ cd MGC
    ```



## Compilation & Run

To compile, use makefile

```sh
    $ cd build
    $ cmake ..
    $ make
```

To run, use the below script:

```bash
    cd build
    ./compression <input_file_path>
```



    
## Tested system specification

Ubuntu 18.04.1 LTS (64-bit) Intel Xeon CPUs (E5-2643 v3 @ 3.4 GHz, 6 cores) 128 GB RAM 

## Compression Methods Settings for Best Compression Ratio
1. 7-zip: Uses LZMA2 with maximum compression (-mx = 9), an 8 GB dictionary, and multithreading enabled.
2. paq8: Applies paq8 compression (e.g., using -12 flag) for high compression ratios.
3. bsc: Creates a tar archive and then compresses it with bsc using the -e2 option.
4. gzip: Uses gzip with maximum compression (-9).
5. zstd: Uses zstd with the -k flag to keep the source file.
6. bzip2: Uses bzip2 with maximum compression (-9).
7. zpaq: Uses zpaq at method level 5 with threads set to twice the number of CPUs.
8. cmix: Uses cmix compression.
