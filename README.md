
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
    $ sudo apt intall gzip
    ```
3. Other general purpose executables come with main repository


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



    
