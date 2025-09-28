# C++ Social Network Friend Recommendation Engine

This project is a C++ application that models a social network from a dataset and recommends new friends to a user. The core logic is based on finding "friends of friends" and ranking them by the number of mutual connections.

This project was built to demonstrate proficiency in graph traversal algorithms and core C++ concepts.

---

## ✨ Features

* **Graph Representation**: Loads a social network from a text file (edge list) into an efficient in-memory graph structure.
* **Friend Recommendation**: Implements a Breadth-First Search (BFS) algorithm to find all users at a distance of 2 ("friends of friends").
* **Ranked Suggestions**: Enhances the recommendation logic by ranking the suggested friends based on the number of mutual friends they share with the user.
* **Error Handling**: Includes basic checks for missing files and non-existent users.

---

## 🛠️ Tech Stack

* **Language**: C++ (C++17)

---

## 🚀 Getting Started

Follow these instructions to compile and run the project on your local machine.

### Prerequisites

You must have a C++ compiler installed and configured on your system (e.g., g++, Clang, or the MSVC compiler from Visual Studio).

### Compilation

1.  Open your terminal or command prompt in the project's root directory.
2.  Run the following command to compile all the necessary source files into a single executable named `SocialMediaRecommendation`.

    ```bash
    g++ SocialMediaRecommendation.cpp Graph.cpp -o SocialMediaRecommendation -std=c++17
    ```
    *Note: If you are using Visual Studio, you can typically add all the `.cpp` files to a project and use the "Build" button.*

---

## 🏃 How to Run

After successful compilation, an executable file will be created in your project directory.

1.  Run the program from your terminal:

    ```bash
    # On Windows
    .\SocialMediaRecommendation.exe

    # On macOS / Linux
    ./SocialMediaRecommendation
    ```

2.  The program will then prompt you to enter the name of the dataset file. Type `facebook_combined.txt` and press Enter to see the recommendations.

---

## 📂 Project Structure

```
.
├── Graph.h                      # Header for the Graph class
├── Graph.cpp                    # Implementation of the Graph class
├── SocialMediaRecommendation.cpp    # Main application entry point and RecommendationEngine class
└── facebook_combined.txt        # The dataset
```