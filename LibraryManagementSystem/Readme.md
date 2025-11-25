<div align="center">

# 📚 Library Management System

**Two approaches to efficient book cataloging and retrieval**

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-11%2B-00599C?logo=cplusplus&logoColor=white" alt="C++11+">
  <img src="https://img.shields.io/badge/Hash%20Table-Custom%20%2B%20STL-FF6B6B" alt="Hash Table">
  <img src="https://img.shields.io/badge/Collision%20Handling-Chaining-orange" alt="Chaining">
  <img src="https://img.shields.io/badge/Status-Completed-success" alt="Completed">
</p>

A comprehensive library management system with **custom hash table** and **STL map** implementations. Demonstrates collision handling, CRUD operations, and persistent file storage.

</div>

---

## ✨ Features

<table>
<tr>
<td width="50%">

### 📖 CRUD Operations
- **Create** — Add books with duplicate ID prevention
- **Read** — Search by ID with full details
- **Update** — Modify title, author, or checkout status
- **Delete** — Remove books with proper memory cleanup

</td>
<td width="50%">

### 🔄 Advanced Features
- **Persistent Storage** — Auto-save/load from text files
- **Checkout System** — Track book availability
- **Collision Handling** — Chaining with linked lists
- **Input Validation** — Error handling for edge cases

</td>
</tr>
</table>

---

## 🏗️ Architecture Overview

### Hash Table Structure (Custom Implementation)

```
Hash Table (size = 100)
┌─────┬─────┬─────┬─────┬─────┐
│  0  │  1  │  2  │ ... │ 99  │  (Buckets)
└──┬──┴─────┴──┬──┴─────┴─────┘
   │           │
   ▼           ▼
 [101]      [202]  →  [302]  →  NULL  (Chaining via Linked Lists)
   │           │          │
 Book A      Book B    Book C
```

**Hash Function:** `h(key) = (key × 31) % 100`

---

## 🛠️ Two Implementations Compared

<table>
<tr>
<th width="50%">🔗 Custom Hash Table</th>
<th width="50%">🗺️ STL Unordered Map</th>
</tr>
<tr>
<td>

**File:** `LMSUsingLinkedList.cpp`

```cpp
struct HashNode {
    int val;
    Book books;
    HashNode* next;
};

vector<HashNode*> table;
```

**Pros:**
- ✅ Full control over implementation
- ✅ Educational value
- ✅ Custom hash function
- ✅ Manual memory management

**Cons:**
- ❌ More code complexity
- ❌ Manual cleanup required

</td>
<td>

**File:** `LMSUSingMap.cpp`

```cpp
unordered_map<int, Book> library;
```

**Pros:**
- ✅ Production-ready
- ✅ Automatic memory management
- ✅ Optimized performance
- ✅ Less code to maintain

**Cons:**
- ❌ Less learning opportunity
- ❌ Black-box implementation

</td>
</tr>
</table>

---

## ⚡ Performance Analysis

| Operation | Time Complexity | Space Complexity | Notes |
|-----------|----------------|------------------|-------|
| **Insert** | O(1) average, O(n) worst | O(1) | Worst case when all keys collide |
| **Search** | O(1) average, O(n) worst | O(1) | Linear search in collision chain |
| **Update** | O(1) average, O(n) worst | O(1) | Search + modify |
| **Delete** | O(1) average, O(n) worst | O(1) | Search + unlink node |
| **Load from File** | O(n) | O(n) | n = number of books |
| **Save to File** | O(n + m) | O(1) | n = books, m = table size |

**Load Factor:** Books / Table Size (optimal: 0.7-0.8)

---

## 🚀 Quick Start

### 🔨 Compilation

```bash
# Custom Hash Table Implementation
g++ -o lms_hash LMSUsingLinkedList.cpp -std=c++11

# STL Map Implementation  
g++ -o lms_map LMSUSingMap.cpp -std=c++11
```

### 💡 Usage Example

```bash
$ ./lms_hash
Enter The File Name You Want To Read/Write To (e.g., library.txt): library.txt
Library data loaded successfully!

--- Library Menu (Custom Hash Table) ---
1. Add A Book
2. Find A Book
3. Update A Book
4. Remove A Book
Enter The Choice: 2

Enter The Id: 101
--- Book Found ---
Title: To Kill a Mockingbird
Author: Harper Lee
Checkout Status: No
```

### 🎯 Interactive Demo

| Action | Command Flow |
|--------|-------------|
| **Add Book** | Choice 1 → Enter ID → Enter Title → Enter Author |
| **Search** | Choice 2 → Enter ID → View Details |
| **Update** | Choice 3 → Enter ID → Select Field → Enter New Value |
| **Delete** | Choice 4 → Enter ID → Confirm Removal |
| **Exit** | Any other number → Auto-saves to file |

---

## 📁 File Format Specification

**library.txt** (50 books included)

```
101                          ← Book ID (integer)
To Kill a Mockingbird        ← Title (string, can have spaces)
Harper Lee                   ← Author (string, can have spaces)
0                            ← Checkout Status (0 = available, 1 = checked out)
102
1984
George Orwell
1
...
```

**Format Rules:**
- Each book occupies exactly 4 lines
- ID must be unique (enforced by program)
- Status: `0` = Available, `1` = Checked Out
- File is auto-generated on exit

**Sample Dataset:** Includes 50 classic books (IDs 101-150)

---

## 📂 Project Structure

```
LibraryManagementSystem/
├── 📄 LMSUsingLinkedList.cpp    # Custom hash table (350 lines)
├── 📄 LMSUSingMap.cpp           # STL unordered_map implementation
├── 📊 library.txt               # Persistent storage (50 books)
└── 📖 Readme.md                 # This file
```

### Key Components

| Component | Responsibility |
|-----------|----------------|
| **Book** | Struct holding title, author, checkout status |
| **HashNode** | Linked list node for collision chaining |
| **HashTable** | Core class with CRUD + file I/O methods |
| **hashFunction()** | Maps book ID to bucket index |
| **librarySystem()** | Main menu loop and user interaction |

---

## 🎓 Learning Outcomes

<div align="center">

**Hash Tables** • **Collision Resolution** • **Linked Lists**  
**File I/O** • **Memory Management** • **CRUD Design Patterns**  
**STL Containers** • **Input Validation** • **Destructor Design**

### 🔑 Key Concepts Demonstrated

</div>

| Concept | Implementation Detail |
|---------|----------------------|
| **Chaining** | Linked lists at each bucket for collision handling |
| **Hash Function** | Multiplicative hashing: `(key × 31) % 100` |
| **Memory Safety** | Destructor traverses all chains to delete nodes |
| **Duplicate Prevention** | Pre-insertion check for existing IDs |
| **Persistent Storage** | Load on startup, save on exit |

---

<div align="center">

### 📊 Dataset Statistics

**50 Classic Books** | **IDs: 101-150** | **Authors: 35+**  
Includes works by Orwell, Tolkien, Hemingway, Austen, and more

---

*Part of the [DSA Projects Roadmap](../README.md) — Phase 1, Project #5*

**📚 Happy Reading! 🚀**

</div>
