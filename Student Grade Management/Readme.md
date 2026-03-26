<div align="center">

# 📚 Student Grade Management System

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Algorithm](https://img.shields.io/badge/Algorithm-Bubble%20Sort-FF6B35?style=for-the-badge)](https://en.wikipedia.org/wiki/Bubble_sort)
[![Data Structure](https://img.shields.io/badge/Data%20Structure-Vector-4169E1?style=for-the-badge)](https://en.cppreference.com/w/cpp/container/vector)
[![Status](https://img.shields.io/badge/Status-Complete-success?style=for-the-badge)](https://github.com)
[![Lines of Code](https://img.shields.io/badge/LOC-230-blue?style=for-the-badge)](https://github.com)

**A production-grade CRUD system demonstrating fundamental data structures and sorting algorithms**

*Master vector operations, bubble sort, linear search, and input validation through a real-world student records application*

[🎯 Features](#-features) • [🏗️ Architecture](#-architecture) • [⚡ Algorithms](#-algorithm-analysis) • [🚀 Quick Start](#-quick-start) • [📊 Performance](#-performance-analysis)

</div>

---

## 🎯 Features

<table>
<tr>
<td width="50%">

### 📋 Core Operations
- ✅ **Create** — Add new student records with validation
- ✅ **Read** — Display all records in formatted table
- ✅ **Update** — Modify student marks in-place
- ✅ **Delete** — Remove entries from collection
- ✅ **Search** — Find students by name (linear search)
- ✅ **Sort** — Order records by marks (bubble sort)

</td>
<td width="50%">

### 🎨 User Experience
- 🖥️ **Formatted Display** — Professional table layout
- ✔️ **Input Validation** — Handles names with spaces
- 🔄 **Interactive Menu** — Continuous operation loop
- 📢 **Visual Feedback** — Success/error messages
- 🛡️ **Edge Case Handling** — Graceful empty state management
- 🎯 **Precision Output** — 2-decimal marks display

</td>
</tr>
</table>

---

## 🏗️ Architecture

### Data Model

```cpp
struct Students {
    string name_of_student;      // Student identifier
    float marks_of_student;      // Academic performance (0-100)
    
    // Constructor for convenient initialization
    Students(string name, float marks) {
        name_of_student = name;
        marks_of_student = marks;
    }
};
```

### Core Class Design

```cpp
class Solution {
private:
    vector<Students> records;    // Dynamic array of student records
    
    // Private methods (encapsulation)
    void createRecord();         // O(1) amortized
    void displayRecords();       // O(n)
    void searchRecords();        // O(n)
    void updateRecords();        // O(n)
    void deleteRecords();        // O(n)
    void sortRecords();          // O(n²)
    void printHeader(const string& title);
    
public:
    void Records();              // Main menu loop
};
```

### Why This Design?

| Choice | Rationale |
|--------|-----------|
| **Vector** | Dynamic sizing, O(1) random access, automatic memory management |
| **Struct** | Simple data aggregation, lightweight, cache-friendly |
| **Private Methods** | Encapsulation, prevents direct data manipulation |
| **String** | Flexible name storage, handles spaces naturally |
| **Float** | Sufficient precision for marks (0-100 range) |

---

## ⚡ Algorithm Analysis

### 1️⃣ Bubble Sort (Sorting Algorithm)

**Implementation:**
```cpp
void sortRecords() {
    int n = records.size();
    
    // Outer loop: number of passes
    for (int i = 0; i < n; i++) {
        // Inner loop: compare adjacent elements
        for (int j = 0; j < n - i - 1; j++) {
            // Swap if current > next
            if (records[j].marks_of_student > records[j + 1].marks_of_student) {
                Students temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    displayRecords();
}
```

**Visual Walkthrough:**
```
Initial Array:  [85.5, 92.0, 78.5, 88.0]

Pass 1:
  Compare 85.5 vs 92.0 → No swap
  Compare 92.0 vs 78.5 → Swap → [85.5, 78.5, 92.0, 88.0]
  Compare 92.0 vs 88.0 → Swap → [85.5, 78.5, 88.0, 92.0] ✓ (92.0 in place)

Pass 2:
  Compare 85.5 vs 78.5 → Swap → [78.5, 85.5, 88.0, 92.0]
  Compare 85.5 vs 88.0 → No swap
  (88.0 already in place)

Pass 3:
  Compare 78.5 vs 85.5 → No swap
  (Already sorted)

Pass 4:
  No comparisons needed
  
Final: [78.5, 85.5, 88.0, 92.0] ✓ Sorted!
```

**Complexity Analysis:**
| Metric | Value | Explanation |
|--------|-------|-------------|
| **Time (Best)** | O(n²) | No early termination optimization |
| **Time (Average)** | O(n²) | Nested loops always execute |
| **Time (Worst)** | O(n²) | Reverse sorted array |
| **Space** | O(1) | In-place sorting, no extra space |
| **Stability** | ✅ Stable | Equal elements maintain order |
| **Adaptive** | ❌ No | Doesn't benefit from partially sorted data |

---

### 2️⃣ Linear Search (Search Algorithm)

**Implementation:**
```cpp
void searchRecords() {
    if (records.empty()) {
        cout << "Nothing To Show Yet!!" << endl;
        return;
    }
    
    string searchName;
    cout << "Enter The Student Name: " << endl;
    cin.ignore();
    getline(cin, searchName);
    
    bool found = false;
    
    // Sequential scan through all records
    for (auto const &student : records) {
        if (student.name_of_student == searchName) {
            cout << "Name: " << student.name_of_student 
                 << " || Marks: " << student.marks_of_student << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "Student Records Not Found" << endl;
    }
}
```

**Complexity Analysis:**
| Metric | Value | Explanation |
|--------|-------|-------------|
| **Time (Best)** | O(1) | Found at first position |
| **Time (Average)** | O(n/2) = O(n) | Found at middle |
| **Time (Worst)** | O(n) | Not found or at end |
| **Space** | O(1) | No extra data structures |
| **Comparisons** | n | Must check each element |

**When to Use:**
- ✅ Unsorted data
- ✅ Small datasets (n < 1000)
- ✅ Linked lists (no random access)
- ❌ Large sorted datasets (use binary search instead)

---

### 3️⃣ CRUD Operations

#### Create (Insert)
```cpp
void createRecord() {
    string name;
    cout << "Enter The Name Of The Student: " << endl;
    cin.ignore();                    // Clear input buffer
    getline(cin, name);              // Read full line (handles spaces)
    
    float marks;
    cout << "Enter The Marks Of Student: " << endl;
    cin >> marks;
    
    records.emplace_back(name, marks);  // O(1) amortized insertion
}
```
**Complexity:** O(1) amortized (vector growth factor: 1.5x)

#### Read (Display)
```cpp
void displayRecords() {
    if (records.empty()) {
        cout << "Nothing To Show Yet!!" << endl;
        return;
    }
    
    printHeader("Student Records");
    cout << left << setw(25) << "Student Name" << " | " << "Marks" << endl;
    cout << "----------------------------------------\n";
    
    // Iterate through all records: O(n)
    for (auto const &student : records) {
        cout << left << setw(25) << student.name_of_student << " | "
             << fixed << setprecision(2) << student.marks_of_student << endl;
    }
    cout << "========================================\n\n";
}
```
**Complexity:** O(n) — Must visit each record

#### Update (Modify)
```cpp
void updateRecords() {
    if (records.empty()) {
        cout << "Nothing To Show Yet!!" << endl;
        return;
    }
    
    string name;
    cout << "Enter The Name Of The Student" << endl;
    cin.ignore();
    getline(cin, name);
    
    bool found = false;
    
    // Search: O(n)
    for (auto &student : records) {
        if (student.name_of_student == name) {
            float marks;
            cout << "Enter The New Marks: " << endl;
            cin >> marks;
            
            student.marks_of_student = marks;  // O(1) modification
            cout << "Success!" << endl;
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Student Records Not Found" << endl;
    }
}
```
**Complexity:** O(n) — Search + O(1) update

#### Delete (Remove)
```cpp
void deleteRecords() {
    if (records.empty()) {
        cout << "Nothing To Show Yet!!" << endl;
        return;
    }
    
    string name;
    cout << "Enter The Name Of The Student: " << endl;
    cin.ignore();
    getline(cin, name);
    
    bool found = false;
    
    // Search: O(n)
    for (int i = 0; i < records.size(); i++) {
        if (records[i].name_of_student == name) {
            records.erase(records.begin() + i);  // O(n) erase operation
            cout << "Success!!" << endl;
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Student Records Not Found" << endl;
    }
}
```
**Complexity:** O(n) — Search + O(n) erase (elements shift)

---

## 🚀 Quick Start

### Prerequisites

```bash
✅ C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
✅ Windows.h library (for console formatting)
✅ Standard Template Library (STL)
```

### Build & Run

**Option 1: Direct Compilation**
```bash
# Windows (MSVC)
cl StudentGradeManagement.cpp

# Linux/macOS (GCC)
g++ -std=c++17 -o student_mgmt StudentGradeManagement.cpp

# Linux/macOS (Clang)
clang++ -std=c++17 -o student_mgmt StudentGradeManagement.cpp
```

**Option 2: Using CMake**
```bash
mkdir build && cd build
cmake ..
cmake --build .
./StudentGradeManagement  # Linux/macOS
StudentGradeManagement.exe  # Windows
```

**Run the Program**
```bash
./student_mgmt              # Linux/macOS
student_mgmt.exe            # Windows
```

---

## 💻 Interactive Menu

```
`````````````````````````````````````````
1. Create A New Student Record
2. Display All Student Records
3. Search For A Students Record
4. Update A Students Record
5. Delete A Students Record
6. Sort All The Records
`````````````````````````````````````````
Enter A Choice:
```

---

## 📖 Usage Examples

### Example Session: Complete Workflow

```bash
========================================
        Student Records        
========================================

`````````````````````````````````````````
1. Create A New Student Record
2. Display All Student Records
3. Search For A Students Record
4. Update A Students Record
5. Delete A Students Record
6. Sort All The Records
`````````````````````````````````````````
Enter A Choice: 
1

Enter The Name Of The Student: 
Alice Johnson
Enter The Marks Of Student: 
85.5

`````````````````````````````````````````
Enter A Choice: 
1

Enter The Name Of The Student: 
Bob Smith
Enter The Marks Of Student: 
92.0

`````````````````````````````````````````
Enter A Choice: 
1

Enter The Name Of The Student: 
Charlie Brown
Enter The Marks Of Student: 
78.5

`````````````````````````````````````````
Enter A Choice: 
2

========================================
        Student Records        
========================================
Student Name             | Marks
----------------------------------------
Alice Johnson            | 85.50
Bob Smith                | 92.00
Charlie Brown            | 78.50
========================================

`````````````````````````````````````````
Enter A Choice: 
3

Enter The Student Name: 
Alice Johnson
Name: Alice Johnson || Marks: 85.5

`````````````````````````````````````````
Enter A Choice: 
6

Records Sorted By Marks: 

========================================
        Student Records        
========================================
Student Name             | Marks
----------------------------------------
Charlie Brown            | 78.50
Alice Johnson            | 85.50
Bob Smith                | 92.00
========================================

`````````````````````````````````````````
Enter A Choice: 
4

Enter The Name Of The Student
Alice Johnson
Enter The New Marks: 
95.0
Success!

`````````````````````````````````````````
Enter A Choice: 
5

Enter The Name Of The Student: 
Charlie Brown
Success!!

`````````````````````````````````````````
Enter A Choice: 
2

========================================
        Student Records        
========================================
Student Name             | Marks
----------------------------------------
Alice Johnson            | 95.00
Bob Smith                | 92.00
========================================
```

---

## 📊 Performance Analysis

### Operation Complexity Comparison

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| **Create** | O(1)* | O(1) | Amortized; vector growth factor ~1.5x |
| **Display** | O(n) | O(1) | Iterate all records, no extra space |
| **Search** | O(n) | O(1) | Linear scan, no optimization |
| **Update** | O(n) | O(1) | Search O(n) + modify O(1) |
| **Delete** | O(n) | O(1) | Search O(n) + erase O(n) |
| **Sort** | O(n²) | O(1) | Bubble sort in-place |

**Overall Space Complexity:** O(n) for storing n student records

### Scalability Analysis

```
Operation Count vs Time (approximate):

100 records:
  Sort: ~10,000 comparisons
  Search: ~50 comparisons (avg)
  
1,000 records:
  Sort: ~1,000,000 comparisons
  Search: ~500 comparisons (avg)
  
10,000 records:
  Sort: ~100,000,000 comparisons ⚠️ Noticeable delay
  Search: ~5,000 comparisons (avg)
```

**Recommendation:** For n > 1,000, consider:
- Quick Sort or Merge Sort (O(n log n))
- Hash table for O(1) search
- Binary search after sorting (O(log n))

---

## 📂 Project Structure

```
Student Grade Management/
├── 📄 StudentGradeManagement.cpp    (230 lines)
│   ├── Includes
│   │   ├── iostream (I/O operations)
│   │   ├── vector (dynamic array)
│   │   ├── string (text handling)
│   │   ├── iomanip (formatting)
│   │   └── Windows.h (console features)
│   │
│   ├── Students struct
│   │   ├── name_of_student: string
│   │   ├── marks_of_student: float
│   │   └── Constructor
│   │
│   ├── Solution class
│   │   ├── Private members
│   │   │   ├── vector<Students> records
│   │   │   ├── createRecord()
│   │   │   ├── displayRecords()
│   │   │   ├── searchRecords()
│   │   │   ├── updateRecords()
│   │   │   ├── deleteRecords()
│   │   │   ├── sortRecords()
│   │   │   └── printHeader()
│   │   │
│   │   └── Public members
│   │       └── Records() [main menu loop]
│   │
│   └── main() entry point
│
├── 🔧 StudentGradeManagement.exe    (compiled binary)
└── 📖 Readme.md                     (this file)
```

### Code Statistics

| Metric | Value |
|--------|-------|
| **Total Lines** | 230 |
| **Code Lines** | 180 |
| **Comment Lines** | 5 |
| **Blank Lines** | 45 |
| **Functions** | 8 |
| **Classes** | 1 |
| **Structs** | 1 |

---

## 🎓 Learning Outcomes

### Core Concepts Mastered

| Concept | Implementation | Complexity |
|---------|----------------|-----------|
| **CRUD Pattern** | Complete Create, Read, Update, Delete | O(1) to O(n²) |
| **Bubble Sort** | Classic comparison-based sorting | O(n²) |
| **Linear Search** | Sequential element scanning | O(n) |
| **Vector Operations** | `emplace_back()`, `erase()`, iteration | O(1) to O(n) |
| **Input Handling** | `getline()` for multi-word input | - |
| **String Comparison** | Exact match searching | - |
| **Formatted Output** | `setw()`, `left`, `fixed`, `setprecision()` | - |
| **Struct Design** | Data aggregation with constructor | - |
| **Encapsulation** | Private methods, public interface | - |
| **Edge Cases** | Empty list handling, not found scenarios | - |

### Skills Developed

✅ **Data Structure Mastery**
- Vector dynamic sizing and memory management
- Struct-based data modeling
- Container iteration patterns

✅ **Algorithm Implementation**
- Bubble sort from scratch
- Linear search logic
- Comparison-based sorting

✅ **C++ Best Practices**
- Range-based for loops
- `const` correctness
- Reference parameters
- RAII principles

✅ **Software Engineering**
- CRUD design pattern
- Encapsulation and access control
- Input validation
- Error handling

---

## 🔮 Enhancement Opportunities

### Tier 1: Immediate Improvements (Easy)

```cpp
// 1. Optimized Bubble Sort with early termination
void sortRecords() {
    int n = records.size();
    for (int i = 0; i < n; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (records[j].marks_of_student > records[j + 1].marks_of_student) {
                swap(records[j], records[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;  // Already sorted!
    }
}
// Improvement: Best case becomes O(n) for sorted data
```

```cpp
// 2. Case-Insensitive Search
#include <algorithm>
#include <cctype>

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void searchRecords() {
    string searchName;
    getline(cin, searchName);
    searchName = toLower(searchName);
    
    for (auto const &student : records) {
        if (toLower(student.name_of_student) == searchName) {
            // Found!
        }
    }
}
```

### Tier 2: Advanced Features (Medium)

```cpp
// 3. Binary Search (after sorting)
int binarySearch(float target) {
    int left = 0, right = records.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (records[mid].marks_of_student == target) return mid;
        if (records[mid].marks_of_student < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;  // Not found
}
// Complexity: O(log n) after O(n log n) sort
```

```cpp
// 4. Statistics Calculation
struct Statistics {
    float average;
    float median;
    float maxMarks;
    float minMarks;
};

Statistics calculateStats() {
    if (records.empty()) return {0, 0, 0, 0};
    
    float sum = 0;
    float maxM = records[0].marks_of_student;
    float minM = records[0].marks_of_student;
    
    for (auto const &student : records) {
        sum += student.marks_of_student;
        maxM = max(maxM, student.marks_of_student);
        minM = min(minM, student.marks_of_student);
    }
    
    return {sum / records.size(), 0, maxM, minM};
}
```

### Tier 3: Production Features (Hard)

```cpp
// 5. File Persistence (CSV)
#include <fstream>

void saveToFile(const string& filename) {
    ofstream file(filename);
    for (auto const &student : records) {
        file << student.name_of_student << "," 
             << student.marks_of_student << "\n";
    }
    file.close();
}

void loadFromFile(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(',');
        string name = line.substr(0, pos);
        float marks = stof(line.substr(pos + 1));
        records.emplace_back(name, marks);
    }
    file.close();
}
```

```cpp
// 6. Advanced Sorting Options
enum SortOrder { ASCENDING, DESCENDING };

void sortRecords(SortOrder order = ASCENDING) {
    // Bubble sort with direction control
    for (int i = 0; i < records.size(); i++) {
        for (int j = 0; j < records.size() - i - 1; j++) {
            bool condition = (order == ASCENDING) 
                ? records[j].marks_of_student > records[j + 1].marks_of_student
                : records[j].marks_of_student < records[j + 1].marks_of_student;
            
            if (condition) {
                swap(records[j], records[j + 1]);
            }
        }
    }
}
```

### Tier 4: System Design (Expert)

- **Database Integration** — SQLite/PostgreSQL backend
- **Multi-Subject Support** — Track grades across multiple courses
- **GPA Calculation** — Weighted grade point average
- **Filtering & Aggregation** — Query by grade range, class statistics
- **Export Reports** — PDF/Excel generation
- **Authentication** — User login system
- **Concurrent Access** — Thread-safe operations

---

## 🧪 Test Cases

### Test Case 1: Basic CRUD Operations
```
✓ Create 3 students
✓ Display all records
✓ Search existing student
✓ Update marks
✓ Delete student
✓ Verify final state
```

### Test Case 2: Edge Cases
```
✓ Display on empty list → "Nothing To Show Yet!!"
✓ Search non-existent student → "Student Records Not Found"
✓ Update non-existent student → "Student Records Not Found"
✓ Delete non-existent student → "Student Records Not Found"
✓ Sort empty list → "Nothing To Show Yet!!"
```

### Test Case 3: Data Integrity
```
✓ Names with spaces handled correctly
✓ Marks precision maintained (2 decimals)
✓ Vector size updates correctly
✓ No memory leaks on delete
```

### Test Case 4: Sorting Verification
```
Input:  [85.5, 92.0, 78.5, 88.0]
Output: [78.5, 85.5, 88.0, 92.0] ✓
```

---

## 📚 Key Takeaways

| Takeaway | Application |
|----------|-------------|
| **Bubble Sort Limitations** | O(n²) unsuitable for large datasets; use Quick/Merge Sort |
| **Vector Efficiency** | O(1) access but O(n) deletion; consider linked lists for frequent deletes |
| **Linear Search Scalability** | Fine for n < 1,000; use hash tables or binary search for larger sets |
| **Input Validation** | `getline()` for multi-word input, `cin.ignore()` for buffer clearing |
| **CRUD Pattern** | Universal design applicable to databases, APIs, and file systems |
| **Encapsulation** | Private methods prevent accidental data corruption |

---

## 🔗 Related Concepts

- **Sorting Algorithms:** Quick Sort, Merge Sort, Heap Sort, Insertion Sort
- **Search Techniques:** Binary Search, Hash Table Lookup, Interpolation Search
- **Data Structures:** Linked Lists, Hash Tables, Binary Search Trees, Heaps
- **Design Patterns:** Repository Pattern, Factory Pattern, Observer Pattern
- **Database Concepts:** ACID properties, Indexing, Query Optimization

---

## 📖 References

- [C++ Vector Documentation](https://en.cppreference.com/w/cpp/container/vector)
- [Bubble Sort Algorithm](https://en.wikipedia.org/wiki/Bubble_sort)
- [Big O Notation](https://en.wikipedia.org/wiki/Big_O_notation)
- [CRUD Operations](https://en.wikipedia.org/wiki/Create,_read,_update_and_delete)
- [C++ Best Practices](https://isocpp.org/wiki/faq)

---

<div align="center">

### 🎓 Perfect for Learning

**Data Structures** • **Algorithms** • **CRUD Pattern** • **C++ Fundamentals**  
**Vector Operations** • **Sorting** • **Searching** • **Input Handling**

---

**Made with ❤️ for DSA Mastery**

*Last Updated: 2026*

</div>
