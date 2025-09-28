# Huffman File Compressor

A command-line utility written in C++ for lossless file compression and decompression using the Huffman coding algorithm. This project is a practical implementation of key data structures and algorithms, including priority queues (min-heaps), binary trees, and bit manipulation for efficient data storage.

![Huffman Tree Visualization](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d8/Huffman_tree_2.svg/1200px-Huffman_tree_2.svg.png)
*<p align="center">An example of a Huffman Tree. Characters with higher frequencies are closer to the root.</p>*

---

## Features

- **Compression**: Compresses any given file by generating optimal prefix codes for each character.
- **Decompression**: Perfectly restores the original file from its compressed version.
- **Binary Format**: Reads and writes data in binary mode to ensure file integrity across all platforms and file types.
- **Header Metadata**: Stores a frequency table in the compressed file's header, allowing it to be self-contained and easily decompressed.
- **Efficient**: Uses a `std::priority_queue` to efficiently build the Huffman tree and `std::unordered_map` for quick code lookups.

---

## How It Works

1.  **Frequency Analysis**: The compressor reads the input file to count the frequency of each character.
2.  **Tree Construction**: A priority queue (min-heap) is used to build the Huffman tree. Nodes with the lowest frequencies are iteratively merged until a single root node remains.
3.  **Code Generation**: The tree is traversed to generate a unique, variable-length binary code for each character. More frequent characters receive shorter codes.
4.  **Encoding & Writing**: The original file is read again, and each character is replaced with its new binary code. These bits are packed into bytes and written to the output file, along with a header containing the frequency map needed for decompression.
5.  **Decoding**: The decompressor reads the header to rebuild the exact same Huffman tree. It then reads the compressed data bit by bit, traversing the tree to find the original characters and write them to the output file.

---

## Getting Started

### Prerequisites

- A C++ compiler that supports C++11 or later (e.g., GCC, Clang, MSVC).
- `make` (optional, but recommended for easy compilation).

### Compilation

You can compile the project using the provided `huffman_coder.cpp` file.

**Using a C++ compiler directly:**

```sh
# For GCC/Clang on Linux or macOS
g++ -std=c++11 -o huffman_coder huffman_coder.cpp

# For MSVC on Windows (in a developer command prompt)
cl /EHsc /std:c++14 huffman_coder.cpp
```

This will create an executable file named `huffman_coder` (or `huffman_coder.exe` on Windows).

---

## Usage

The program is run from the command line and will prompt you for the desired operation.

**To run the program:**

```sh
./huffman_coder
```

You will be presented with a menu:

```
--- Huffman File Compressor ---
1. Compress a file
2. Decompress a file
Enter your choice (1 or 2):
```

### Example

Let's say you have a file named `sample.txt`.

**1. Compress `sample.txt` to `compressed.bin`:**

```sh
$ ./huffman_coder
--- Huffman File Compressor ---
1. Compress a file
2. Decompress a file
Enter your choice (1 or 2): 1
Enter the input file name: sample.txt
Enter the output file name: compressed.bin
Compressing...
Compression successful. Original characters: 1500
Operation completed.
```

**2. Decompress `compressed.bin` to `restored.txt`:**

```sh
$ ./huffman_coder
--- Huffman File Compressor ---
1. Compress a file
2. Decompress a file
Enter your choice (1 or 2): 2
Enter the input file name: compressed.bin
Enter the output file name: restored.txt
Decompressing...
Decompression successful. Decoded 1500 characters.
Operation completed.
```

After decompression, you can verify that `restored.txt` is identical to the original `sample.txt` using a diff tool:

```sh
# This command should produce no output if the files are identical
diff sample.txt restored.txt
```

---

## Code Structure

- **`TreeNode` struct**: Represents a node in the Huffman tree, storing a character, its frequency, and pointers to its children.
- **`CompareNodes` struct**: A custom comparator for the priority queue to order `TreeNode` pointers by frequency (creating a min-heap).
- **`HuffmanCoder` class**: Encapsulates all the logic for compression and decompression.
  - `compress(inputFile, outputFile)`: The main public method for compression.
  - `decompress(inputFile, outputFile)`: The main public method for decompression.
  - Private helper methods for building the frequency map, constructing the tree, generating codes, and cleaning up memory.
- **`main()` function**: Provides the command-line user interface for interacting with the `HuffmanCoder`.