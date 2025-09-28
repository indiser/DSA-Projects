<div align="center">

# 🧩 Maze Solver

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Algorithm-BFS_DFS-4CAF50?style=for-the-badge" alt="BFS DFS">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Two pathfinding algorithms to navigate through mazes*

</div>

---

## 🚀 Features

### 🎯 Dual Implementation
- 🔍 **BFS Solver** - Finds shortest path guaranteed
- 🌊 **DFS Solver** - Explores depth-first with backtracking
- 📊 **Path Visualization** - Shows solution route
- ⚡ **Interactive Input** - Custom maze creation
- 🎨 **Visual Output** - Clear path marking

### 🗺️ Maze Navigation
- Start point: `S` | End point: `E`
- Walls: `#` | Open paths: `.`
- Solution path: `*`

---

## 🛠️ Algorithm Comparison

<table>
<tr>
<td width="50%">

### 🔍 BFS (Breadth-First Search)
**File:** `MazeSolverBFS.cpp`

- **Strategy:** Level-by-level exploration
- **Guarantee:** Shortest path found
- **Data Structure:** Queue (FIFO)
- **Time:** O(V + E)
- **Space:** O(V)

</td>
<td width="50%">

### 🌊 DFS (Depth-First Search)
**File:** `MazeSolverDFS.cpp`

- **Strategy:** Deep exploration with backtracking
- **Guarantee:** Any valid path
- **Data Structure:** Stack/Recursion
- **Time:** O(V + E)
- **Space:** O(V)

</td>
</tr>
</table>

---

## 🎮 Usage

### Compilation
```bash
# BFS Implementation
g++ -o maze_bfs MazeSolverBFS.cpp

# DFS Implementation  
g++ -o maze_dfs MazeSolverDFS.cpp
```

### Sample Maze
```
S . . # . . E
# . # # . # .
. . . . . # .
# # . # . . .
. . . # # . .
```

### Sample Output
```
🧩 Maze solved using BFS:

S * * # . . E
# * # # . # *
. * . . . # *
# # * # . . *
. . * # # * *

✅ Path found! Length: 12 steps
```

---

## 🔄 Algorithm Flow

### BFS Process
1. **Initialize** queue with start position
2. **Explore** all adjacent cells level by level
3. **Mark** visited cells to avoid cycles
4. **Track** parent cells for path reconstruction
5. **Return** shortest path when end is reached

### DFS Process
1. **Start** from initial position
2. **Explore** one direction completely
3. **Backtrack** when hitting dead end
4. **Mark** visited cells during exploration
5. **Return** first valid path found

---

## 🎯 Learning Objectives

- ✅ Graph traversal algorithms (BFS vs DFS)
- ✅ Queue and stack data structures
- ✅ Pathfinding and backtracking techniques
- ✅ 2D array manipulation and navigation
- ✅ Algorithm complexity analysis

---

## 🔮 Extensions

- 🎲 **Random maze generation**
- 🏃 **A* algorithm** for optimal pathfinding
- 🎨 **GUI visualization** with graphics
- 📊 **Performance benchmarking**
- 🧭 **Multiple path solutions**

---

<div align="center">

**🧩 Navigate Your Way Out! 🗺️**

</div>