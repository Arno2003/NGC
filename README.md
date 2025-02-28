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

The project supports the following general-purpose algorithm:

- 7zip
- paq8
- bsc
- gzip
- zstd
- bzip2
- zpaq
- cmix



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

6. bzip2, 7zip and some other compressors requires large system memory (~48 GB RAM).


## Installation

1. Clone the repository:
    ```sh
   $ git clone https://github.com/Arno2003/finalYear_journal

   $ cd finalYear_journal
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



    
## Test system specification:

ubuntu-linux (32C, 128 GB RAM). 

## Compression Methods Settings for Best Compression Ratio:
1. 7zip: Uses LZMA2 with maximum compression (-mx=9), an 8GB dictionary, and multithreading enabled.
2. PAQ8: Applies PAQ8 compression (e.g., using -12 flag) for high compression ratios.
3. BSC: Creates a tar archive and then compresses it with BSC using the -e2 option.
4. GZIP: Uses gzip with maximum compression (-9).
5. ZSTD: Uses ZSTD with the -k flag to keep the source file.
6. BZIP2: Uses bzip2 with maximum compression (-9).
7. ZPAQ: Uses zpaq at method level 5 with threads set to twice the number of CPUs.
8. CMIX: Uses CMIX compression.
