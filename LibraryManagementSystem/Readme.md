<div align="center">

# 📚 Library Management System

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Data_Structure-Hash_Table-FF6B6B?style=for-the-badge" alt="Hash Table">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Two implementations of a library management system showcasing different data structures*

</div>

---

## 🚀 Features

### 📖 Core Operations
- ➕ **Add Books** - Insert new books with unique IDs
- 🔍 **Search Books** - Find books by ID with detailed info
- ✏️ **Update Books** - Modify title, author, or checkout status
- 🗑️ **Remove Books** - Delete books from the system
- 💾 **File I/O** - Save/load library data to/from text files

### 🔄 Checkout System
- 📋 Track book availability status
- 🔄 Toggle checkout/return functionality
- 📊 Real-time status updates

---

## 🛠️ Two Implementations

<table>
<tr>
<td width="50%">

### 🔗 Custom Hash Table
**File:** `LMSUsingLinkedList.cpp`

- **Structure:** Hash table with chaining
- **Collision Handling:** Linked lists
- **Hash Function:** `(key * 31) % table_size`
- **Memory:** Dynamic allocation

</td>
<td width="50%">

### 🗺️ STL Map
**File:** `LMSUSingMap.cpp`

- **Structure:** `std::unordered_map`
- **Built-in:** STL implementation
- **Simpler:** Less code complexity
- **Reliable:** Standard library

</td>
</tr>
</table>

---

## 📊 Performance Comparison

| Operation | Custom Hash Table | STL Map |
|-----------|------------------|---------|
| Insert    | O(1) average     | O(1) average |
| Search    | O(1) average     | O(1) average |
| Delete    | O(1) average     | O(1) average |
| Memory    | Manual management| Automatic |

---

## 🎮 Usage

### Compilation
```bash
# Custom Hash Table Implementation
g++ -o lms_hash LMSUsingLinkedList.cpp

# STL Map Implementation  
g++ -o lms_map LMSUSingMap.cpp
```

### Sample Interaction
```
📚 Enter The File Name: library.txt
✅ Library data loaded successfully!

--- Library Menu ---
1. Add A Book
2. Find A Book  
3. Update A Book
4. Remove A Book
Enter Choice: 1

📖 Enter Book ID: 101
📝 Enter Title: The Great Gatsby
👤 Enter Author: F. Scott Fitzgerald
✅ Book added successfully!
```

---

## 📁 Data Format

**library.txt:**
```
143
Heart of Darkness
Joseph Conrad
0
109
The Lord of the Rings
J.R.R. Tolkien
1
```

Format: `ID → Title → Author → Checkout Status (0/1)`

---

## 🎯 Learning Objectives

- ✅ Hash table implementation and collision handling
- ✅ File I/O operations in C++
- ✅ Memory management with pointers
- ✅ STL containers vs custom implementations
- ✅ CRUD operations design patterns

---

<div align="center">

**📚 Happy Reading! 🚀**

</div>