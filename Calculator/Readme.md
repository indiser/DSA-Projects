<div align="center">

# 🧮 Simple Calculator with History

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Data_Structure-Stack-4CAF50?style=for-the-badge" alt="Stack">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Interactive calculator with stack-based history tracking*

</div>

---

## 🚀 Features

### 🔢 Basic Operations
- ➕ **Addition** - Add two numbers
- ➖ **Subtraction** - Subtract two numbers  
- ✖️ **Multiplication** - Multiply two numbers
- ➗ **Division** - Divide with zero-check protection
- 📊 **Result Display** - Immediate calculation output

### 📚 History Management
- 📋 **Stack Storage** - LIFO history tracking
- 🔄 **Complete History** - View all past calculations
- 💾 **Session Memory** - Results stored until exit
- ⚡ **Fast Access** - Stack-based retrieval

---

## 🛠️ Technical Implementation

<table>
<tr>
<td width="50%">

### 📚 Stack Data Structure
- **Container:** `std::stack<float>`
- **Operations:** Push results after each calculation
- **Access:** LIFO (Last In, First Out)
- **Memory:** Dynamic allocation

</td>
<td width="50%">

### 🔧 Core Functions
- **`addition()`** - Basic arithmetic
- **`subtraction()`** - Basic arithmetic
- **`multiplication()`** - Basic arithmetic  
- **`division()`** - With zero-division check

</td>
</tr>
</table>

---

## 🎮 Interactive Menu

```
Press 1 for addition
Press 2 for subtraction
Press 3 for multiplication
Press 4 for division
Press 0 to exit
Enter Your Choice: 1
```

### Sample Calculation
```
Enter The First Number: 15.5
Enter The Second Number: 4.5
Result: 20

Enter Your Choice: 4
Enter The First Number: 10
Enter The Second Number: 0
Error dividing by zero
```

---

## 📊 History Display

```
The Recent History is:
20
15.5
8.75
42
```

**Note:** History displays in reverse order (most recent first) due to stack LIFO nature.

---

## 🔧 Error Handling

### Division by Zero Protection
```cpp
if(b == 0) {
    cout << "Error dividing by zero" << endl;
    continue;  // Skip storing invalid result
}
```

### Input Validation
- **Float Support** - Handles decimal numbers
- **Continuous Operation** - Loop until user exits
- **Invalid Choice** - Graceful menu return

---

## 🎯 Learning Objectives

- ✅ Stack data structure implementation
- ✅ LIFO (Last In, First Out) principle
- ✅ Basic arithmetic operations in C++
- ✅ Error handling and input validation
- ✅ Menu-driven program design
- ✅ Float precision handling

---

## 🚀 Usage

### Compilation & Run
```bash
g++ -o calculator SimpleCalculatorWithHistory.cpp
./calculator
```

### Sample Session
```
Enter Your Choice: 1
Enter The First Number: 25
Enter The Second Number: 15
Result: 40

Enter Your Choice: 0

The Recent History is:
40
```

---

## 🔮 Future Enhancements

- 🧮 **Advanced Operations** - Power, square root, trigonometry
- 📁 **File Storage** - Save history to file
- 🔄 **History Navigation** - Search and reuse previous results
- 🎨 **GUI Interface** - Visual calculator design
- 📊 **Expression Parsing** - Handle complex expressions

---

<div align="center">

**🧮 Calculate with Confidence! 📊**

</div>