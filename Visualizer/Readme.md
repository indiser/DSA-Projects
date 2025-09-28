<div align="center">

# 📊 Sorting Algorithm Visualizer

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Algorithms-3_Sorts-4CAF50?style=for-the-badge" alt="3 Sorts">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Real-time visualization of sorting algorithms with animated console output*

</div>

---

## 🚀 Features

### 🎯 Three Sorting Algorithms
- 🔀 **Merge Sort** - Divide and conquer approach
- ⚡ **Quick Sort** - Partition-based sorting
- 🏔️ **Heap Sort** - Binary heap implementation
- 📊 **Visual Bars** - ASCII bar chart representation
- ⏱️ **Real-time Animation** - Step-by-step visualization

### 🎨 Visual Experience
- 📈 **Bar Charts** - Height represents array values
- 🔄 **Live Updates** - See every swap and comparison
- 🎭 **Clear Console** - Smooth animation effect
- ⏸️ **Controlled Speed** - 50ms delay between steps

---

## 🛠️ Algorithm Comparison

<table>
<tr>
<td width="33%">

### 🔀 Merge Sort
- **Time:** O(n log n)
- **Space:** O(n)
- **Stable:** Yes
- **Method:** Divide & Conquer

</td>
<td width="33%">

### ⚡ Quick Sort
- **Time:** O(n log n) avg
- **Space:** O(log n)
- **Stable:** No
- **Method:** Partitioning

</td>
<td width="33%">

### 🏔️ Heap Sort
- **Time:** O(n log n)
- **Space:** O(1)
- **Stable:** No
- **Method:** Binary Heap

</td>
</tr>
</table>

---

## 🎮 Interactive Menu

```
```````````````````````````````````````
1. Merge Sort
2. Quick Sort
3. Heap Sort
Enter The Choice:
```````````````````````````````````````
```

### Visual Output Example
```
10 | ██████████
9  | █████████
8  | ████████
7  | ███████
6  | ██████
5  | █████
4  | ████
3  | ███
```

---

## 🔧 Technical Implementation

### Array Visualization
```cpp
void displayArray(const vector<int> &arr) {
    clearConsole();
    for (int val : arr) {
        cout << val << " | ";
        for (int i = 0; i < val; i++) {
            cout << "█";
        }
        cout << endl;
    }
}
```

### Animation Control
- **Clear Console** - Windows API for smooth updates
- **Sleep Delay** - 50ms between visualization steps
- **UTF-8 Support** - Unicode block characters for bars

---

## 🎯 Algorithm Details

### Merge Sort Process
1. **Divide** array into two halves
2. **Recursively sort** both halves
3. **Merge** sorted halves back together
4. **Visualize** each merge operation

### Quick Sort Process
1. **Choose pivot** (last element)
2. **Partition** around pivot
3. **Recursively sort** partitions
4. **Visualize** each swap operation

### Heap Sort Process
1. **Build max heap** from array
2. **Extract maximum** repeatedly
3. **Heapify** after each extraction
4. **Visualize** heap operations

---

## 🎯 Learning Objectives

- ✅ Understanding sorting algorithm mechanics
- ✅ Time and space complexity analysis
- ✅ Recursive algorithm implementation
- ✅ Console-based visualization techniques
- ✅ Comparative algorithm performance

---

## 🚀 Usage

### Compilation & Run
```bash
g++ -o visualizer SortAlgoVisualization.cpp
./visualizer
```

### Sample Session
```
Enter The Choice: 1

[Animated visualization of merge sort]

After Merge Sort
3 4 5 6 7 8 9 10
```

---

## 🔮 Future Enhancements

- 🎨 **Color Coding** - Different colors for comparisons/swaps
- 📊 **Performance Metrics** - Time and comparison counters
- 🎛️ **Custom Arrays** - User-defined input arrays
- 📈 **More Algorithms** - Bubble sort, selection sort, radix sort
- 🖥️ **GUI Version** - Graphical interface with better visuals

---

<div align="center">

**📊 Watch Algorithms Come to Life! 🎭**

</div>