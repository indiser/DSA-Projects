<div align="center">

# 📚 Student Grade Management System

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Algorithm-Bubble_Sort-FF6B35?style=for-the-badge" alt="Bubble Sort">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Complete CRUD system for managing student records with sorting capabilities*

</div>

---

## 🚀 Features

### 📋 CRUD Operations
- ➕ **Create Records** - Add new student entries
- 👀 **Display Records** - View all students in formatted table
- 🔍 **Search Records** - Find students by name
- ✏️ **Update Records** - Modify student marks
- 🗑️ **Delete Records** - Remove student entries
- 📊 **Sort Records** - Order by marks (ascending)

### 🎨 User Interface
- 🌈 **Colorful Menu** - Enhanced visual experience
- 📋 **Formatted Display** - Clean table layout
- ⚡ **Interactive Prompts** - User-friendly navigation
- 🔄 **Continuous Loop** - Multiple operations per session

---

## 🛠️ Data Structure & Algorithms

<table>
<tr>
<td width="50%">

### 📊 Student Structure
```cpp
struct Students {
    string name_of_student;
    float marks_of_student;
}
```

</td>
<td width="50%">

### 🔄 Bubble Sort
- **Time Complexity:** O(n²)
- **Space Complexity:** O(1)
- **Method:** Adjacent swapping
- **Order:** Ascending by marks

</td>
</tr>
</table>

---

## 🎮 Menu Interface

```
`````````````````````````````````````````
1. Create A New Student Record
2. Display All Student Records
3. Search For A Students Record
4. Update A Students Record
5. Delete A Students Record
6. Sort All The Records
`````````````````````````````````````````
```

### Sample Operations

#### ➕ Adding Student
```
Enter The Name Of The Student: John Doe
Enter The Marks Of Student: 85.5
✅ Student added successfully!
```

#### 📊 Display Records
```
========================================
        Student Records        
========================================
Student Name             | Marks
----------------------------------------
John Doe                 | 85.50
Jane Smith               | 92.00
========================================
```

---

## 🔧 Technical Implementation

### Core Functions
- **`createRecord()`** - Input validation and record creation
- **`displayRecords()`** - Formatted table output with headers
- **`searchRecords()`** - Linear search by student name
- **`updateRecords()`** - Find and modify existing records
- **`deleteRecords()`** - Remove records with vector erase
- **`sortRecords()`** - Bubble sort implementation

### Memory Management
- **Vector Container** - Dynamic array for student records
- **String Handling** - `getline()` for names with spaces
- **Input Validation** - Proper data type handling

---

## 🎯 Learning Objectives

- ✅ CRUD operations design patterns
- ✅ Bubble sort algorithm implementation
- ✅ Linear search techniques
- ✅ Vector manipulation and memory management
- ✅ User interface design in console applications
- ✅ Input validation and error handling

---

## 🚀 Usage

### Compilation & Run
```bash
g++ -o student_mgmt StudentGradeManagement.cpp
./student_mgmt
```

### Sample Session
```
Enter choice: 1
Enter The Name Of The Student: Alice Johnson
Enter The Marks Of Student: 78.5

Enter choice: 6
Records Sorted By Marks:
Alice Johnson | 78.50
John Doe      | 85.50
Jane Smith    | 92.00
```

---

## 🔮 Future Enhancements

- 📁 **File I/O** - Save/load records from files
- 🔢 **Multiple Subjects** - Track grades across subjects
- 📊 **Advanced Sorting** - Quick sort, merge sort options
- 📈 **Grade Analytics** - Average, median, grade distribution
- 🎓 **GPA Calculation** - Automatic grade point computation

---

<div align="center">

**📚 Manage Student Success! 🎓**

</div>