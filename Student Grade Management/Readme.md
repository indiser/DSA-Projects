<div align="center">

# 📚 Student Grade Management System

**Complete CRUD system with sorting and search capabilities**

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-11%2B-00599C?logo=cplusplus&logoColor=white" alt="C++11+">
  <img src="https://img.shields.io/badge/Algorithm-Bubble%20Sort-FF6B35" alt="Bubble Sort">
  <img src="https://img.shields.io/badge/Data%20Structure-Vector-blue" alt="Vector">
  <img src="https://img.shields.io/badge/Status-Completed-success" alt="Completed">
</p>

A comprehensive student record management system implementing fundamental CRUD operations, linear search, and bubble sort algorithm. Perfect for learning basic data structures and algorithms.

</div>

---

## ✨ Features

<table>
<tr>
<td width="50%">

### 📋 CRUD Operations
- **Create** — Add new student records
- **Read** — Display all records in formatted table
- **Update** — Modify student marks
- **Delete** — Remove student entries
- **Search** — Find students by name
- **Sort** — Order by marks (ascending)

</td>
<td width="50%">

### 🎨 User Experience
- **Formatted Display** — Clean table layout
- **Input Validation** — Handles names with spaces
- **Interactive Menu** — Continuous operation loop
- **Visual Feedback** — Success/error messages
- **Empty State Handling** — Graceful empty list messages

</td>
</tr>
</table>

---

## 🏗️ Architecture Overview

### Data Structure

```cpp
struct Students {
    string name_of_student;
    float marks_of_student;
    
    Students(string name, float marks) {
        name_of_student = name;
        marks_of_student = marks;
    }
};

class Solution {
private:
    vector<Students> records;  // Dynamic array of student records
    // CRUD methods...
};
```

**Why Vector?**
- Dynamic resizing (no fixed capacity)
- O(1) access by index
- Easy insertion/deletion with `emplace_back()` and `erase()`
- Automatic memory management

---

## 🔍 Algorithm Deep Dive

### 1. Bubble Sort Implementation

```cpp
void sortRecords() {
    int n = records.size();
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (records[j].marks_of_student > records[j+1].marks_of_student) {
                // Swap adjacent elements
                Students temp = records[j];
                records[j] = records[j+1];
                records[j+1] = temp;
            }
        }
    }
}
```

**Visualization:**
```
Initial: [85.5, 92.0, 78.5, 88.0]

Pass 1:  [85.5, 78.5, 88.0, 92.0]  ← 92.0 bubbles to end
Pass 2:  [78.5, 85.5, 88.0, 92.0]  ← 88.0 in position
Pass 3:  [78.5, 85.5, 88.0, 92.0]  ← Already sorted
Pass 4:  [78.5, 85.5, 88.0, 92.0]  ← Complete!
```

**Complexity:**
- **Time:** O(n²) — Nested loops
- **Space:** O(1) — In-place sorting
- **Best Case:** O(n²) — No optimization for sorted arrays
- **Worst Case:** O(n²) — Reverse sorted array

### 2. Linear Search

```cpp
void searchRecords() {
    string searchName;
    getline(cin, searchName);
    
    bool found = false;
    for (const auto& student : records) {
        if (student.name_of_student == searchName) {
            cout << "Name: " << student.name_of_student 
                 << " || Marks: " << student.marks_of_student;
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Student Records Not Found";
    }
}
```

**Complexity:** O(n) — Must check each record sequentially

### 3. Delete Operation

```cpp
void deleteRecords() {
    string name;
    getline(cin, name);
    
    for (int i = 0; i < records.size(); i++) {
        if (records[i].name_of_student == name) {
            records.erase(records.begin() + i);  // O(n) operation
            cout << "Success!!";
            break;
        }
    }
}
```

**Complexity:** O(n) — Search + O(n) erase = O(n) total

---

## 🎮 Interactive Menu

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

## 🚀 Quick Start

### Prerequisites

```bash
✅ C++11 compatible compiler (g++, Clang, MSVC)
✅ Windows.h (for console formatting on Windows)
```

### 🔨 Build & Run

```bash
# Compile
g++ -o student_mgmt StudentGradeManagement.cpp -std=c++11

# Run
./student_mgmt              # Linux/macOS
student_mgmt.exe            # Windows
```

---

## 💡 Usage Examples

### Example Session

```bash
Enter A Choice: 1
Enter The Name Of The Student: 
Alice Johnson
Enter The Marks Of Student: 
85.5

Enter A Choice: 1
Enter The Name Of The Student: 
Bob Smith
Enter The Marks Of Student: 
92.0

Enter A Choice: 2

========================================
        Student Records        
========================================
Student Name             | Marks
----------------------------------------
Alice Johnson            | 85.50
Bob Smith                | 92.00
========================================

Enter A Choice: 3
Enter The Student Name: 
Alice Johnson
Name: Alice Johnson || Marks: 85.5

Enter A Choice: 6
Records Sorted By Marks: 

========================================
        Student Records        
========================================
Student Name             | Marks
----------------------------------------
Alice Johnson            | 85.50
Bob Smith                | 92.00
========================================

Enter A Choice: 4
Enter The Name Of The Student
Alice Johnson
Enter The New Marks: 
95.0
Success!

Enter A Choice: 5
Enter The Name Of The Student: 
Bob Smith
Success!!
```

---

## ⚡ Performance Analysis

### Operation Complexity

| Operation | Time Complexity | Space Complexity | Notes |
|-----------|----------------|------------------|-------|
| **Create** | O(1) amortized | O(1) | Vector `emplace_back()` |
| **Display** | O(n) | O(1) | Iterate all records |
| **Search** | O(n) | O(1) | Linear search |
| **Update** | O(n) | O(1) | Search + modify |
| **Delete** | O(n) | O(1) | Search + erase |
| **Sort** | O(n²) | O(1) | Bubble sort in-place |

**Overall Space:** O(n) for storing n student records

---

## 📂 Project Structure

```
Student Grade Management/
├── 📄 StudentGradeManagement.cpp    # Main implementation (230 lines)
│   ├── Students struct              # Data model
│   └── Solution class               # CRUD operations
├── 🔧 StudentGradeManagement.exe    # Compiled executable
└── 📖 Readme.md                     # This file
```

### Key Components

| Component | Responsibility |
|-----------|----------------|
| **Students** | Struct holding student name and marks |
| **Solution** | Main class encapsulating all operations |
| **createRecord()** | Add new student with input validation |
| **displayRecords()** | Formatted table output with headers |
| **searchRecords()** | Linear search by name |
| **updateRecords()** | Find and modify marks |
| **deleteRecords()** | Remove record from vector |
| **sortRecords()** | Bubble sort by marks (ascending) |
| **printHeader()** | Utility for formatted output |

---

## 🎓 Learning Outcomes

<div align="center">

**CRUD Operations** • **Bubble Sort** • **Linear Search**  
**Vector Manipulation** • **Struct Design** • **Input Handling**  
**Console Formatting** • **Loop Control** • **Error Handling**

</div>

### Key Concepts Demonstrated

| Concept | Implementation |
|---------|----------------|
| **CRUD Pattern** | Complete Create, Read, Update, Delete operations |
| **Bubble Sort** | Classic O(n²) sorting with adjacent swaps |
| **Linear Search** | Sequential search through vector |
| **Vector Operations** | `emplace_back()`, `erase()`, range-based loops |
| **Input Validation** | `getline()` for names with spaces, `cin.ignore()` |
| **Formatted Output** | `setw()`, `left`, `fixed`, `setprecision()` |
| **Struct Usage** | Custom data type with constructor |

---

## 🔮 Potential Enhancements

### Immediate Improvements
- **Optimized Bubble Sort** — Add early termination flag
- **Binary Search** — After sorting, use O(log n) search
- **Case-Insensitive Search** — Convert to lowercase for comparison

### Advanced Features
- **File Persistence** — Save/load records from CSV/JSON
- **Multiple Subjects** — Track grades across subjects
- **GPA Calculation** — Automatic grade point average
- **Grade Distribution** — Histogram of marks
- **Advanced Sorting** — Quick sort, merge sort options
- **Filtering** — Show students above/below threshold
- **Statistics** — Average, median, standard deviation
- **Export Reports** — Generate formatted reports

---

## 📊 Sample Data Scenarios

### Test Case 1: Basic Operations
```
Add: John (85.5), Alice (92.0), Bob (78.5)
Display: Shows 3 records
Search: "Alice" → Found (92.0)
Update: "Bob" → 88.0
Sort: [Bob (78.5→88.0), John (85.5), Alice (92.0)]
Delete: "John"
Final: [Bob (88.0), Alice (92.0)]
```

### Test Case 2: Edge Cases
```
Display on empty list → "Nothing To Show Yet!!"
Search non-existent → "Student Records Not Found"
Delete from empty → "Nothing To Show Yet!!"
Sort single record → Works correctly
```

---

<div align="center">

### 📈 System Statistics

**Data Structure:** Vector (Dynamic Array)  
**Sorting Algorithm:** Bubble Sort (O(n²))  
**Search Method:** Linear Search (O(n))

---

*Part of the [DSA Projects Roadmap](../README.md) — Phase 1, Project #4*

**📚 Manage Student Success! 🎓**

</div>
