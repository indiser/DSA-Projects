<div align="center">

# ✏️ Spell Checker with Trie

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Data_Structure-Trie-FF6B35?style=for-the-badge" alt="Trie">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Advanced spell checker with auto-completion and smart suggestions using Trie data structure*

</div>

---

## 🚀 Features

### ✏️ Core Functionality
- ✅ **Spell Checking** - Verify word correctness
- 🔍 **Auto-Completion** - Suggest word completions
- 💡 **Smart Suggestions** - Generate correction candidates
- 📚 **Dictionary Management** - Load from file, add new words
- 🔤 **Prefix Matching** - Find words starting with given prefix

### 🧠 Intelligent Corrections
- **Deletion** - Remove one character
- **Insertion** - Add one character  
- **Substitution** - Replace one character
- **Transposition** - Swap adjacent characters

---

## 🛠️ Trie Implementation

<table>
<tr>
<td width="50%">

### 🌳 Trie Structure
```cpp
class TrieNode {
    bool isEndOfWord;
    unordered_map<char, TrieNode*> mp;
}
```

</td>
<td width="50%">

### ⚡ Performance
- **Insert:** O(L) where L = word length
- **Search:** O(L) 
- **Auto-complete:** O(L + N) where N = results
- **Space:** O(ALPHABET_SIZE × N × M)

</td>
</tr>
</table>

---

## 🎮 Interactive Experience

### Spell Check Mode
```
> hello
✓ 'hello' is spelled correctly!

> helo
✗ 'helo' not found.
Did you mean:
  - hello
  - help
  - held
```

### Auto-Completion Mode
```
> prog
Auto-completions for 'prog':
  - program
  - programming
  - progress
  - project
```

---

## 🔧 Core Operations

### Dictionary Loading
```cpp
bool populateFromDictionary(const string &filename)
```
- Loads words from text file
- Builds trie structure efficiently
- Handles file I/O errors gracefully

### Suggestion Generation
```cpp
vector<string> getSuggestions(const string &word)
```
- **4 Edit Operations:** Deletion, insertion, substitution, transposition
- **Candidate Generation:** Creates all possible single-edit variations
- **Validation:** Checks candidates against dictionary

---

## 📁 File Structure

### Dictionary Format
```
hello
world
programming
computer
algorithm
```
**Format:** One word per line (plain text)

### Usage Flow
1. **Load Dictionary** - Enter filename (e.g., `dictionary.txt`)
2. **Enter Words** - Type words or prefixes
3. **Get Results** - Receive corrections or completions
4. **Add Words** - Expand dictionary dynamically

---

## 🎯 Learning Objectives

- ✅ Trie data structure implementation
- ✅ String matching and prefix algorithms
- ✅ Edit distance and spell correction techniques
- ✅ File I/O operations in C++
- ✅ Memory management with pointers
- ✅ Recursive tree traversal

---

## 🚀 Usage

### Compilation & Run
```bash
g++ -o spell_checker SpellCheckerUsingTrie.cpp
./spell_checker
```

### Sample Session
```
Enter The FileName: dictionary.txt
Smart Spell Checker Ready!
Dictionary loaded successfully.

> programing
✗ 'programing' not found.
Did you mean:
  - programming

> prog
Auto-completions for 'prog':
  - program
  - programming
  - progress
```

---

## 🔮 Future Enhancements

- 📊 **Frequency-based Suggestions** - Rank by word popularity
- 🌐 **Multiple Languages** - Support for different dictionaries
- 🎯 **Context-aware Corrections** - Consider surrounding words
- 📱 **Real-time Checking** - As-you-type validation
- 🔤 **Phonetic Matching** - Sound-based suggestions

---

<div align="center">

**✏️ Write with Confidence! 📝**

</div>