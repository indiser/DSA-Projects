# 🗺️ Travel Route Planner

A C++ implementation of graph pathfinding algorithms (Dijkstra's and A*) for finding optimal routes between Indian cities.

## 📋 Project Overview

This project demonstrates two popular pathfinding algorithms:
- **Dijkstra's Algorithm**: Finds shortest paths from a source to all other nodes
- **A* Algorithm**: Uses heuristic search to find optimal path between two cities

## 🎯 Core Concepts

- **Graph Algorithms**: Dijkstra's, A* Search
- **Data Structures**: Priority Queue, Adjacency List, Hash Maps
- **Heuristic Function**: Haversine formula for calculating great-circle distance
- **File I/O**: Reading city locations and route data from files

## 📁 Files

- `main.cpp` - Main implementation with Dijkstra and A* algorithms
- `locations.txt` - 200 Indian cities with coordinates (ID, Name, Latitude, Longitude)
- `routes.txt` - Road connections between cities with distances in km
- `prototype.cpp` - Initial prototype with multiple pathfinding algorithms

## 🚀 How to Run

### Compile
```bash
g++ -o travel main.cpp
```

### Execute
```bash
./travel
```

## 💡 Usage

1. Choose algorithm:
   - **Option 1**: Dijkstra's Algorithm (enter source node ID)
   - **Option 2**: A* Algorithm (enter source and destination city names)

2. For A*, enter city names like:
   ```
   Source: Kolkata
   Destination: Delhi
   ```

## 🧮 Algorithm Details

### Dijkstra's Algorithm
- **Time Complexity**: O((V + E) log V)
- **Space Complexity**: O(V)
- Finds shortest path from source to all nodes

### A* Algorithm
- **Time Complexity**: O(E log V) (best case)
- **Space Complexity**: O(V)
- Uses f(n) = g(n) + h(n) where:
  - g(n) = actual distance from start
  - h(n) = heuristic (straight-line distance using Haversine)

### Haversine Formula
Calculates great-circle distance between two points on Earth:
```
a = sin²(Δlat/2) + cos(lat1) × cos(lat2) × sin²(Δlon/2)
c = 2 × atan2(√a, √(1-a))
distance = R × c  (R = 6371 km)
```

## 📊 Dataset

- **Cities**: 200 major Indian cities
- **Routes**: Major highway connections with actual road distances
- **Coverage**: All states and union territories

## 🔧 Technical Features

- Dynamic graph size detection
- City name-based search (A*)
- Error handling for invalid inputs
- Formatted console output with Unicode symbols

## 📝 Example Output

```
========== A* ALGORITHM ==========
Source: Kolkata -> Destination: Delhi
==================================
Path: Kolkata -> Patna -> Lucknow -> Delhi
Total Distance: 1530 km
Path Length: 4 nodes
==================================
```

## 🎓 Learning Outcomes

- Graph representation and traversal
- Priority queue operations
- Heuristic search optimization
- File parsing and data loading
- Real-world algorithm application

## 📌 Future Enhancements

- Add more pathfinding algorithms (Bellman-Ford, Floyd-Warshall)
- Implement bidirectional search
- Add visualization of routes
- Include traffic and time-based routing
- Support for multiple route options

## 👨‍💻 Author

Part of DSA Projects - FAANG Interview Preparation Roadmap

**Project #12**: Travel Route Planner (Phase 3: Advanced Intermediate)
