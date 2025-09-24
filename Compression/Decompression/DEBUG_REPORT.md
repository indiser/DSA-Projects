# Huffman Compression Tool - Debug Report

## Issues Found and Fixed

### 1. **Missing Include Statement**
**Problem:** Missing `#include <sstream>` header
**Fix:** Added the missing include for string stream operations

### 2. **Syntax Errors**
**Problem:** Extra semicolon in continue statement
```cpp
continue;
;  // Extra semicolon
```
**Fix:** Removed the extra semicolon

### 3. **Typos**
**Problem:** 
- "Error Occured" → "Error Occurred" 
- Variable name "charecter" → "character"
**Fix:** Corrected spelling errors

### 4. **Non-Deterministic Tree Construction (CRITICAL)**
**Problem:** The `CompareNodes` comparator only compared frequencies, leading to non-deterministic tree construction when frequencies were equal. This caused different Huffman trees during compression vs decompression.

**Original Code:**
```cpp
struct CompareNodes {
    bool operator()(TreeNode *left, TreeNode *right) {
        return left->freq > right->freq;  // Only frequency comparison
    }
};
```

**Fix:** Added character comparison for deterministic ordering:
```cpp
struct CompareNodes {
    bool operator()(TreeNode *left, TreeNode *right) {
        if (left->freq != right->freq)
            return left->freq > right->freq;
        return left->data > right->data;  // Deterministic tie-breaking
    }
};
```

### 5. **Tree Reconstruction Mismatch (CRITICAL)**
**Problem:** The decompression was rebuilding the Huffman tree from frequency data, but due to the non-deterministic nature of priority queues with equal frequencies, the reconstructed tree could differ from the original compression tree, leading to incorrect decoding.

**Fix:** Store and retrieve Huffman codes directly instead of rebuilding the tree:

**Compression - Store codes:**
```cpp
// Store Huffman codes
outFile << "CODES" << endl;
for (const auto &pair : codeMaps) {
    outFile << (int)(unsigned char)pair.first << ":" << pair.second << endl;
}
```

**Decompression - Read codes:**
```cpp
// Read Huffman codes
unordered_map<string, char> codeToChar;
getline(inFile, line);
if (line != "CODES") {
    cout << "Error: Missing codes section in compressed file." << endl;
    return;
}

for (const auto &pair : freqMap) {
    getline(inFile, line);
    size_t colon_pos = line.find(':');
    if (colon_pos == string::npos) continue;
    
    int ascii_code = stoi(line.substr(0, colon_pos));
    string code = line.substr(colon_pos + 1);
    char character = static_cast<char>(ascii_code);
    codeToChar[code] = character;
}
```

### 6. **Improved Decompression Algorithm**
**Problem:** Tree traversal during decompression was error-prone and complex.

**Fix:** Replaced tree traversal with direct code lookup:
```cpp
string current_code = "";
// Build code bit by bit and lookup directly
current_code += (bit == 0) ? '0' : '1';
if (codeToChar.find(current_code) != codeToChar.end()) {
    outFile.put(codeToChar[current_code]);
    decoded_count++;
    current_code = "";
}
```

### 7. **Input Handling Improvements**
**Problem:** Input handling could fail with filenames containing spaces.
**Fix:** Used `getline()` instead of `cin >>` for file paths and added input buffer clearing.

## Root Cause Analysis

The primary issue was **non-deterministic Huffman tree construction**. When multiple characters had the same frequency, the priority queue's ordering was undefined, leading to different tree structures between compression and decompression. This resulted in:

1. Different Huffman codes being generated
2. Incorrect bit-to-character mapping during decompression  
3. Garbled output text

## Solution Summary

The fix involved two key changes:
1. **Deterministic tree building** - Added character-based tie-breaking in comparator
2. **Direct code storage** - Store actual Huffman codes in compressed file instead of relying on tree reconstruction

This ensures the exact same encoding used during compression is available during decompression, eliminating any possibility of decoding errors.