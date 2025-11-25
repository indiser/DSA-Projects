<div align="center">

# 🌐 Social Network Friend Recommendation Engine

**Intelligent friend suggestions powered by graph algorithms**

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white" alt="C++17">
  <img src="https://img.shields.io/badge/Algorithm-BFS-orange" alt="BFS">
  <img src="https://img.shields.io/badge/Data%20Structure-Graph-blueviolet" alt="Graph">
  <img src="https://img.shields.io/badge/Status-Completed-success" alt="Completed">
</p>

A C++ application that models social networks and recommends friends using **Breadth-First Search (BFS)** and **mutual connection analysis**. Demonstrates advanced graph traversal algorithms and efficient data structure design.

</div>

---

## ✨ Features

<table>
<tr>
<td width="50%">

### 🎯 Core Functionality
- **Graph Representation** — Adjacency list using `unordered_map`
- **BFS Traversal** — Find friends-of-friends (distance = 2)
- **Mutual Friends Ranking** — Sort by shared connections
- **Real Dataset Support** — Facebook combined network data

</td>
<td width="50%">

### 🛡️ Robustness
- **File Validation** — Checks for missing/invalid files
- **User Verification** — Validates user existence in graph
- **Efficient Lookups** — O(1) average-case friend retrieval
- **Memory Optimized** — Visited set prevents redundant processing

</td>
</tr>
</table>

---

## 🧪 Testing

The project includes test files for validation:

```bash
# Compile and run tests
g++ simple_test.cpp Graph.cpp -o simple_test -std=c++17
./simple_test
```

---

## 🔍 Algorithm Deep Dive

```
┌─────────────┐
│   User 0    │  (Target User)
└──────┬──────┘
       │
   ┌───┴───┐
   │       │
   ▼       ▼
  [1]     [2]     (Direct Friends - Distance 1)
   │       │
   ├───┬───┤
   │   │   │
   ▼   ▼   ▼
  [3] [4] [5]     (Friends of Friends - Distance 2) ✅ RECOMMENDED
```

### Algorithm Pipeline

1. **📊 Load Graph** — Parse edge list from file into adjacency list
2. **🔎 BFS Traversal** — Explore network up to distance 2 from target user
3. **🚫 Filter Direct Friends** — Exclude already-connected users
4. **🤝 Count Mutual Friends** — Calculate shared connections for each candidate
5. **📈 Rank & Sort** — Order recommendations by mutual friend count (descending)

### Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Graph Construction | O(E) | O(V + E) |
| BFS Traversal | O(V + E) | O(V) |
| Mutual Friends Calculation | O(V × d²) | O(V) |
| Sorting Recommendations | O(R log R) | O(R) |

*Where V = vertices (users), E = edges (friendships), d = avg degree, R = recommendations*

---

## 🛠️ Tech Stack

**Language:** C++17  
**Data Structures:** Adjacency List, Hash Map, Hash Set, Queue  
**Algorithms:** Breadth-First Search (BFS), Sorting  
**Dataset:** Facebook Combined Network (4,039 nodes, 88,234 edges)

---

## 🚀 Quick Start

### Prerequisites

```bash
# Required
✅ C++17 compatible compiler (g++, Clang, MSVC)

# Optional
📦 CMake (for advanced build configuration)
```

### 🔨 Build & Run

```bash
# Clone or navigate to project directory
cd SocialMediaRecommendation

# Compile
g++ SocialMediaRecommendation.cpp Graph.cpp -o SocialMediaRecommendation -std=c++17

# Run
./SocialMediaRecommendation          # Linux/macOS
SocialMediaRecommendation.exe        # Windows
```

### 💡 Example Usage

```bash
$ ./SocialMediaRecommendation
Enter the name of the file: 
facebook_combined.txt
Graph loaded successfully!
Enter The User: 
0

Recommendations for User 0:
107 1684 1912 3437 0 348 ...
```

**Interpretation:** Users are ranked by mutual friends (highest first)

---

## 📂 Project Structure

```
SocialMediaRecommendation/
├── 📄 Graph.h                          # Graph class interface
├── 📄 Graph.cpp                        # Graph implementation (adjacency list)
├── 📄 SocialMediaRecommendation.cpp    # RecommendationEngine + main()
├── 📊 facebook_combined.txt            # Facebook network dataset
├── 📊 test_combined.txt                # Test dataset
├── 🧪 simple_test.cpp                  # Unit tests
└── 📖 Readme.md                        # This file
```

### Key Components

| Component | Responsibility |
|-----------|----------------|
| **Graph** | Stores adjacency list, provides friend lookup & user validation |
| **RecommendationEngine** | BFS traversal, mutual friend calculation, ranking logic |
| **main()** | File I/O, user interaction, orchestrates recommendation flow |

---

## 🎓 Learning Outcomes

<div align="center">

**Graph Algorithms** • **BFS Traversal** • **Adjacency Lists**  
**Hash Maps** • **Set Operations** • **Ranking Algorithms**  
**File I/O** • **STL Containers** • **Algorithm Optimization**

### 📊 Dataset Information

**Source:** Stanford SNAP (Social Network Analysis Project)  
**Network:** Facebook combined ego-networks  
**Nodes:** 4,039 users | **Edges:** 88,234 friendships

---

*Part of the [DSA Projects Roadmap](../README.md) — Phase 3, Project #11*

</div>
