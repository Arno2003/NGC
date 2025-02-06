
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
- PAQ8
- BSC
- GZIP
- ZSTD
- BZIP2
- lpaq8
- zpaq
- Huffman






## Installation

1. Clone the repository:
    ```sh
   $ git clone https://github.com/Arno2003/finalYear_journal

   $ cd finalYear_journal
    ```



## Compilation & Run

To compile, use the provided compile.sh script:

```bash
 cd main
./compile.sh

```
To run, use the below script:

```bash
 ./test <input_file_path>
```


    
