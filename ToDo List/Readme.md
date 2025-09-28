<div align="center">

# ✅ To-Do List Application

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Data_Structure-Linked_List-FF6B35?style=for-the-badge" alt="Linked List">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Interactive task management system with linked list implementation*

</div>

---

## 🚀 Features

### 📋 Task Management
- ➕ **Create Tasks** - Add new tasks with unique IDs
- 👀 **View Tasks** - Display all tasks with status indicators
- ✏️ **Update Tasks** - Modify task names
- ✅ **Toggle Status** - Mark tasks as complete/incomplete
- 🗑️ **Delete Tasks** - Remove tasks from list

### 🎨 Visual Interface
- 🆔 **Unique IDs** - Auto-incrementing task identifiers
- ✓ **Status Icons** - Visual completion indicators (✓/✗)
- 🌈 **Clean Menu** - User-friendly navigation
- 🔄 **Continuous Loop** - Multiple operations per session

---

## 🛠️ Data Structure Implementation

<table>
<tr>
<td width="50%">

### 📊 Task Structure
```cpp
struct Task {
    int id;
    string name_of_the_task;
    bool is_completed = false;
}
```

</td>
<td width="50%">

### 🔗 Linked List Node
```cpp
struct ListNode {
    Task data;
    ListNode *next;
}
```

</td>
</tr>
</table>

### ⚡ Operations Complexity
- **Insert:** O(n) - Append to end
- **Search:** O(n) - Linear search by ID
- **Update:** O(n) - Find and modify
- **Delete:** O(n) - Find and remove

---

## 🎮 Interactive Menu

```
``````````````````````````````````````````
1. Create a Task 
2. Traverse Or Display The Tasks 
3. Update The List Of Tasks 
4. Change The Status Of A Task
5. Delete A Task
``````````````````````````````````````````
```

### Sample Operations

#### ➕ Creating Task
```
Enter The Task You Wish To Create: Buy groceries
Success!
```

#### 📋 Displaying Tasks
```
Name Of The Tasks:
1. [✗] Buy groceries
2. [✓] Complete homework
3. [✗] Call dentist
```

#### ✅ Status Toggle
```
Enter The Unique Id Of The Task You Want To Change The Status Of: 1
Success!
```

---

## 🔧 Core Functions

### CRUD Operations
- **`createTask()`** - Append new task to linked list
- **`readTask()`** - Traverse and display all tasks
- **`updateTaskName()`** - Find by ID and modify name
- **`changeStatus()`** - Toggle completion status
- **`deleteTask()`** - Remove task and handle memory

### Memory Management
- **Dynamic Allocation** - `new ListNode()` for task creation
- **Proper Cleanup** - `delete` for removed tasks
- **Null Checks** - Safe pointer operations

---

## 🎯 Learning Objectives

- ✅ Linked list implementation and manipulation
- ✅ Dynamic memory allocation in C++
- ✅ CRUD operations design patterns
- ✅ Pointer arithmetic and traversal
- ✅ Menu-driven program architecture
- ✅ Memory leak prevention

---

## 🚀 Usage

### Compilation & Run
```bash
g++ -o todo ToDo.cpp
./todo
```

### Sample Session
```
Enter Your Choice: 1
Enter The Task You Wish To Create: Learn C++
Success!

Enter Your Choice: 2
Name Of The Tasks:
1. [✗] Learn C++

Enter Your Choice: 4
Enter The Unique Id Of The Task You Want To Change The Status Of: 1
Success!

Enter Your Choice: 2
Name Of The Tasks:
1. [✓] Learn C++
```

---

## 🔮 Future Enhancements

- 📅 **Due Dates** - Add deadline tracking
- 🏷️ **Categories** - Organize tasks by type
- 💾 **File Storage** - Save tasks to file
- 🔍 **Search Feature** - Find tasks by keyword
- 📊 **Progress Stats** - Completion analytics
- 🎨 **Priority Levels** - High/medium/low importance

---

<div align="center">

**✅ Stay Organized, Stay Productive! 📋**

</div>