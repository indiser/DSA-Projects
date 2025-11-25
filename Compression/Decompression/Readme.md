<div align="center">

# 🗜️ Huffman File Compressor

**Lossless file compression using optimal prefix codes**

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-11%2B-00599C?logo=cplusplus&logoColor=white" alt="C++11+">
  <img src="https://img.shields.io/badge/Algorithm-Huffman%20Coding-brightgreen" alt="Huffman Coding">
  <img src="https://img.shields.io/badge/Compression-Lossless-blue" alt="Lossless">
  <img src="https://img.shields.io/badge/Status-Completed-success" alt="Completed">
</p>

A command-line utility for efficient file compression and decompression using the Huffman coding algorithm. Implements priority queues (min-heaps), binary trees, and bit manipulation for optimal data storage.

![Huffman Tree Visualization](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d8/Huffman_tree_2.svg/1200px-Huffman_tree_2.svg.png)
*An example of a Huffman Tree. Characters with higher frequencies are closer to the root.*

</div>

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🗜️ **Compression** | Generates optimal prefix codes for each character |
| 📂 **Decompression** | Perfectly restores original files with zero data loss |
| 💾 **Binary Format** | Cross-platform file integrity for all file types |
| 📋 **Self-Contained** | Embedded frequency table for standalone decompression |
| ⚡ **Efficient** | Priority queue + hash map for optimal performance |

---

## 🔧 How It Works

```mermaid
graph LR
    A[Input File] --> B[Frequency Analysis]
    B --> C[Build Huffman Tree]
    C --> D[Generate Codes]
    D --> E[Encode & Compress]
    E --> F[Compressed File]
    F --> G[Read Header]
    G --> H[Rebuild Tree]
    H --> I[Decode Bits]
    I --> J[Original File]
```

### Compression Pipeline

1. 📊 **Frequency Analysis** — Count character occurrences in the input file
2. 🌳 **Tree Construction** — Build Huffman tree using min-heap (priority queue)
3. 🔢 **Code Generation** — Assign variable-length binary codes (frequent chars get shorter codes)
4. 💾 **Encoding & Writing** — Replace characters with binary codes, pack into bytes with header
5. 🔓 **Decoding** — Rebuild tree from header, traverse bit-by-bit to restore original data

---

## 🚀 Getting Started

### Prerequisites

- C++ compiler with C++11+ support (GCC, Clang, or MSVC)
- `make` (optional)

### 🔨 Compilation

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

## 📖 Usage

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

### 💡 Example Workflow

**Scenario:** Compress and decompress `sample.txt`

#### Step 1: Compress

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

#### Step 2: Decompress

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

#### Step 3: Verify Integrity

```sh
diff sample.txt restored.txt  # No output = perfect restoration ✅
```

---

## 🏗️ Code Architecture

```cpp
📦 Huffman Compressor
 ├── 🌳 TreeNode              // Huffman tree node (char, frequency, children)
 ├── ⚖️  CompareNodes          // Min-heap comparator for priority queue
 ├── 🎯 HuffmanCoder           // Core compression/decompression engine
 │   ├── compress()           // Public: Compress file
 │   ├── decompress()         // Public: Decompress file
 │   └── [Private Helpers]    // Frequency map, tree building, code generation
 └── 🖥️  main()                // CLI interface
```

### Key Components

| Component | Purpose |
|-----------|----------|
| `TreeNode` | Huffman tree node with character, frequency, and child pointers |
| `CompareNodes` | Custom comparator for min-heap ordering by frequency |
| `HuffmanCoder` | Encapsulates compression/decompression logic |
| Helper Methods | Frequency analysis, tree construction, code generation, memory cleanup |

---

<div align="center">

### 🎓 Learning Outcomes

**Data Structures:** Priority Queues • Binary Trees • Hash Maps  
**Algorithms:** Greedy Algorithms • Bit Manipulation • Tree Traversal  
**Concepts:** Lossless Compression • Prefix Codes • File I/O

---

*Part of the [DSA Projects Roadmap](../../README.md) — Phase 2, Project #10*

</div>
