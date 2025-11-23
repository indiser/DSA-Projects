# 📈 Stock Price Analysis Tool

A production-grade hybrid C++/Python application for real-time stock market analysis using advanced data structures, algorithms, and technical indicators.

## 📋 Project Overview

This project demonstrates enterprise-level software engineering by combining C++17's performance with Python's ecosystem to deliver:
- **Technical Indicators**: SMA, EMA, RSI calculations
- **Optimal Trading Strategy**: Find best buy/sell points using dynamic programming
- **Top K Analysis**: Identify highest daily gains using min-heap
- **Real-time Data**: Fetch live stock prices from Alpha Vantage API
- **Visualization**: Generate trading charts with matplotlib

## 🎯 Core Concepts

- **Data Structures**: Min-Heap (Priority Queue), Dynamic Arrays
- **Algorithms**: Kadane's Algorithm Variant, Wilder's Smoothing (RSI), Exponential Smoothing (EMA)
- **Design Patterns**: Hybrid Architecture, Separation of Concerns
- **Security**: Input sanitization, path traversal prevention
- **Best Practices**: Professional logging, error handling, type hints

## 📁 Project Structure

```
Stock Analysis/
├── src/
│   ├── analysis.cpp      # C++ algorithm implementations
│   ├── analysis.h        # Function declarations and structs
│   └── bindings.cpp      # pybind11 Python-C++ interface
├── build/                # CMake build output
├── main.py              # Python orchestration and API integration
├── .env                 # API key configuration
├── CMakeLists.txt       # Build configuration
└── system.log           # Application logs

## 🚀 Setup & Installation

### Prerequisites
```bash
# Install Python dependencies
pip install requests python-dotenv matplotlib

# Ensure you have CMake and a C++17 compiler installed
```

### Configuration
1. Create a `.env` file in the project root:
```env
ALPHA_VANTAGE_KEY=your_api_key_here
```
2. Get a free API key from [Alpha Vantage](https://www.alphavantage.co/support/#api-key)

### Build C++ Module
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

### Run Application
```bash
python main.py
```

### Sample Output
```
Enter Stock Symbol (e.g., IBM): IBM
2025-11-23 20:01:22 [INFO] Fetching data for symbol: IBM
2025-11-23 20:01:22 [INFO] Successfully retrieved 100 data points.

========================================
 ANALYSIS REPORT: IBM
========================================
 Latest Price:      $297.44
 SMA (5-day):       $292.70
 EMA (20-day):      $298.88
 RSI (14-day):      50.20
 Top 3 Daily Jumps: ['$22.46', '$13.91', '$7.56']
========================================
 Max Profit: $80.21
   Buy Day:   Index 27 (@ $234.77)
   Sell Day:  Index 92 (@ $314.98)
```

## 💡 Implemented Features

### 1. Simple Moving Average (SMA)
- Calculates average of last N closing prices
- **Algorithm**: Sliding window summation
- **Time Complexity**: O(n)
- **Use Case**: Identify trend direction

### 2. Exponential Moving Average (EMA)
- Weighted average giving more importance to recent prices
- **Algorithm**: Exponential smoothing with multiplier 2/(period+1)
- **Time Complexity**: O(n)
- **Use Case**: More responsive to recent price changes than SMA

### 3. Relative Strength Index (RSI)
- Momentum oscillator measuring speed and magnitude of price changes
- **Algorithm**: Wilder's smoothing method
- **Range**: 0-100 (>70 overbought, <30 oversold)
- **Time Complexity**: O(n)
- **Use Case**: Identify overbought/oversold conditions

### 4. Optimal Buy/Sell Strategy
- Finds maximum profit from single transaction
- **Algorithm**: Modified Kadane's algorithm with index tracking
- **Time Complexity**: O(n)
- **Space Complexity**: O(1)
- **Returns**: Profit amount, buy day index, sell day index

### 5. Top K Daily Gains
- Identifies K largest day-over-day price increases
- **Data Structure**: Min-heap (priority queue)
- **Time Complexity**: O(n log k)
- **Space Complexity**: O(k)
- **Use Case**: Spot high volatility days

## 🧮 Algorithm Details

### Maximum Profit with Index Tracking
```cpp
TradeResult findMaxProfit(const vector<double>& prices) {
    double min_price = prices[0];
    int potential_buy_index = 0;
    double max_profit = 0.0;
    int best_buy_index = -1, best_sell_index = -1;
    
    for (int i = 1; i < prices.size(); ++i) {
        double current_profit = prices[i] - min_price;
        if (current_profit > max_profit) {
            max_profit = current_profit;
            best_sell_index = i;
            best_buy_index = potential_buy_index;
        }
        if (prices[i] < min_price) {
            min_price = prices[i];
            potential_buy_index = i;
        }
    }
    return {max_profit, best_buy_index, best_sell_index};
}
```

### RSI Calculation (Wilder's Method)
```cpp
vector<double> calculateRSI(const vector<double>& prices, int period) {
    // 1. Calculate initial average gain/loss
    for (int i = 1; i <= period; ++i) {
        double change = prices[i] - prices[i-1];
        if (change > 0) avg_gain += change;
        else avg_loss += abs(change);
    }
    avg_gain /= period; avg_loss /= period;
    
    // 2. Apply Wilder's smoothing for subsequent values
    for (int i = period + 1; i < prices.size(); ++i) {
        avg_gain = ((avg_gain * (period-1)) + current_gain) / period;
        avg_loss = ((avg_loss * (period-1)) + current_loss) / period;
        double rs = avg_gain / avg_loss;
        double rsi = 100.0 - (100.0 / (1.0 + rs));
    }
}
```

### Top K Gains (Min-Heap)
```cpp
vector<double> findTopKDailyGains(const vector<double>& prices, int k) {
    priority_queue<double, vector<double>, greater<double>> min_heap;
    for (int i = 1; i < prices.size(); i++) {
        double gain = prices[i] - prices[i-1];
        min_heap.push(gain);
        if (min_heap.size() > k) min_heap.pop();
    }
    return extract_heap_to_vector(min_heap);
}
```

## 📊 Technical Indicators Explained

| Indicator | Value | Interpretation |
|-----------|-------|----------------|
| **SMA** | $292.70 | Average price over last 5 days |
| **EMA** | $298.88 | Recent prices weighted higher; above SMA suggests upward momentum |
| **RSI** | 50.20 | Neutral zone (30-70); not overbought or oversold |
| **Max Profit** | $80.21 | Best single buy/sell opportunity in dataset |

## 🔧 Technical Stack

### Backend (C++17)
- **STL Containers**: `vector`, `priority_queue`
- **Algorithms**: Custom implementations of financial indicators
- **Memory Management**: RAII, smart pointers
- **Build System**: CMake 3.15+

### Frontend (Python 3.8+)
- **API Client**: `requests` with timeout and error handling
- **Visualization**: `matplotlib` for chart generation
- **Configuration**: `python-dotenv` for environment variables
- **Logging**: Built-in `logging` module with file and console handlers

### Integration Layer
- **pybind11**: Zero-overhead C++/Python bindings
- **Type Conversion**: Automatic `std::vector` ↔ Python list conversion
- **Struct Binding**: `TradeResult` exposed as Python object

## 📝 API Reference

### C++ Functions (via Python)

```python
import stock_analyzer

# Calculate Simple Moving Average
sma = stock_analyzer.calculate_sma(prices, window=5)
# Returns: float (average of last 5 prices)

# Calculate Exponential Moving Average
ema_values = stock_analyzer.calculate_ema(prices, period=20)
# Returns: list[float] (EMA for each price point)

# Calculate Relative Strength Index
rsi_values = stock_analyzer.calculate_rsi(prices, period=14)
# Returns: list[float] (RSI values 0-100)

# Find optimal buy/sell strategy
trade = stock_analyzer.find_max_profit(prices)
# Returns: TradeResult(max_profit, buy_day_index, sell_day_index)

# Get top K daily gains
top_gains = stock_analyzer.find_top_k_gains(prices, k=3)
# Returns: list[float] (top 3 daily price increases)
```

## 🎓 Key Learnings & Interview Relevance

### Data Structures
- **Min-Heap**: Efficient top-K element tracking (common in "Top K" problems)
- **Dynamic Arrays**: Vector operations and memory management

### Algorithms
- **Kadane's Algorithm**: Maximum subarray variant for stock profit
- **Sliding Window**: Moving average calculations
- **Exponential Smoothing**: Time series analysis technique

### System Design
- **Hybrid Architecture**: When to use C++ vs Python
- **API Integration**: Rate limiting, error handling, retries
- **Security**: Input validation, path traversal prevention
- **Logging**: Production-grade observability

### Software Engineering
- **Build Systems**: CMake for cross-platform C++ projects
- **Foreign Function Interface**: pybind11 for language interop
- **Configuration Management**: Environment variables, .env files
- **Error Handling**: Graceful degradation, fail-fast principles

## 🐛 Troubleshooting

**Module not found error:**
```bash
# Rebuild the C++ module
cd build
cmake --build . --config Debug
```

**API rate limit:**
- Free tier: 25 requests/day
- Wait 24 hours or upgrade to premium

**Missing API key:**
- Ensure `.env` file exists with `ALPHA_VANTAGE_KEY=your_key`

## 📌 Future Enhancements

- [ ] MACD and Bollinger Bands indicators
- [ ] Multi-stock comparison dashboard
- [ ] SQLite database for historical data caching
- [ ] WebSocket for real-time streaming
- [ ] Backtesting framework with performance metrics
- [ ] REST API with FastAPI
- [ ] Docker containerization

## 📚 References

- [Wilder's RSI Formula](https://school.stockcharts.com/doku.php?id=technical_indicators:relative_strength_index_rsi)
- [EMA Calculation](https://www.investopedia.com/terms/e/ema.asp)
- [Alpha Vantage API Documentation](https://www.alphavantage.co/documentation/)
- [pybind11 Documentation](https://pybind11.readthedocs.io/)

## 👨‍💻 Author

Part of **DSA Projects - FAANG Interview Preparation Roadmap**

**Project #13**: Stock Price Analysis (Phase 3: Advanced Intermediate)

**Status**: ✅ Completed

---

*This project demonstrates production-ready code with enterprise-level practices including security, logging, error handling, and performance optimization.*
