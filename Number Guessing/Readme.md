<div align="center">

# 🎯 Number Guessing Game with Statistics

<p>
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Algorithm-Binary_Search-FF6B35?style=for-the-badge" alt="Binary Search">
  <img src="https://img.shields.io/badge/Status-Complete-28a745?style=for-the-badge" alt="Complete">
</p>

*Interactive guessing game with advanced statistics and search functionality*

</div>

---

## 🚀 Features

### 🎮 Game Mechanics
- 🎲 **Random Number Generation** - MT19937 for true randomness
- 🎯 **Interactive Guessing** - Real-time feedback (Too High/Low)
- 🔄 **Replay System** - Continue playing multiple rounds
- 📊 **Attempt Tracking** - Count and store all guesses

### 📈 Advanced Statistics
- 📋 **Guess History** - Complete record of all attempts
- 🔢 **Sorted Display** - Merge sort implementation
- 🔍 **Search Feature** - Binary search through guess history
- 📊 **Performance Metrics** - Total attempts and success rate

---

## 🛠️ Algorithm Implementation

<table>
<tr>
<td width="50%">

### 🔀 Merge Sort
- **Purpose:** Sort guess history
- **Complexity:** O(n log n)
- **Method:** Divide and conquer
- **Stability:** Maintains order

</td>
<td width="50%">

### 🔍 Binary Search
- **Purpose:** Search guess history
- **Complexity:** O(log n)
- **Requirement:** Sorted array
- **Efficiency:** Logarithmic lookup

</td>
</tr>
</table>

---

## 🎮 Gameplay Flow

### Game Session
```
🎯 RANDOM NUMBERS ARE GENERATED FROM 1 TO 10

Enter your Guess: 5
Too Low!

Enter your Guess: 8
Too High!

Enter your Guess: 7
You Got It!!!!
```

### Statistics Display
```
📊 STATISTICS
The Secret number is: 7
The Number of tries it took: 3
The guesses were: 5 7 8

🔍 QUESTION
Have I Ever Guessed This Number??? 6
No you have not
```

---

## 🔧 Technical Details

### Random Number Generation
- **Engine:** `std::mt19937` (Mersenne Twister)
- **Distribution:** `std::uniform_int_distribution`
- **Seed:** Current time for uniqueness
- **Range:** Configurable bounds (default 1-10)

### Data Structures
- **Vector:** Dynamic array for storing guesses
- **Recursive:** Merge sort and binary search
- **Memory:** Efficient space utilization

---

## 🎯 Learning Objectives

- ✅ Random number generation techniques
- ✅ Merge sort algorithm implementation
- ✅ Binary search on sorted arrays
- ✅ Interactive game loop design
- ✅ Statistical data analysis

---

## 🚀 Usage

### Compilation & Run
```bash
g++ -o number_game NumberGuessingWithStatistics.cpp
./number_game
```

### Customization
```cpp
// Modify range in global variables
int lowerBound = 1, upperBound = 100;  // 1-100 range
```

---

## 🔮 Future Enhancements

- 📊 **Difficulty Levels** - Multiple range options
- 🏆 **High Score System** - Best attempt records
- 📈 **Advanced Analytics** - Guess pattern analysis
- 🎨 **GUI Interface** - Visual game experience
- 🌐 **Multiplayer Mode** - Competitive guessing

---

<div align="center">

**🎯 Test Your Guessing Skills! 🎲**

</div>