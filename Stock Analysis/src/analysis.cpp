#include "analysis.h"
#include <numeric>
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include<math.h>
using namespace std;

int add(int i, int j) {
    return i + j;
}

// Sliding Window SMA - Returns latest window average
// Time Complexity: O(N)
// Space Complexity: O(1)
double calculateSMA(const vector<double>& prices, int window) {
    if (prices.size() < window) return 0.0;
    
    double sum = 0.0;
    for (int i = prices.size() - window; i < prices.size(); ++i) {
        sum += prices[i];
    }
    return sum / window;
}

// double findMaxProfit(const vector<double>& prices) {
//     if (prices.size() < 2) return 0.0;
//     double minPrice = prices[0];
//     double maxProfit = 0.0;
//     for (const auto& price : prices) {
//         maxProfit = max(maxProfit, price - minPrice);
//         minPrice = min(minPrice, price);
//     }
//     return maxProfit;
// }

TradeResult findMaxProfit(const std::vector<double>& prices) {
    if (prices.size() < 2) {
        return {0.0, -1, -1};
    }

    double min_price = prices[0];
    int min_price_index = 0; // Track WHERE the min price happened

    double max_profit = 0.0;
    int best_buy_index = -1;
    int best_sell_index = -1;

    // We need to track the "min_price_index" carefully. 
    // If we find a new min_price, we update our 'potential' buy day.
    // Only when we find a new MAX PROFIT do we lock in the best_buy_index.
    
    int potential_buy_index = 0; 

    for (int i = 1; i < prices.size(); ++i) {
        // Check Profit
        double current_profit = prices[i] - min_price;

        if (current_profit > max_profit) {
            max_profit = current_profit;
            best_sell_index = (int)i;
            best_buy_index = potential_buy_index; // Lock in the buy day associated with this profit
        }

        // Check Price Drop
        if (prices[i] < min_price) {
            min_price = prices[i];
            potential_buy_index = (int)i; // This MIGHT be a buy day if prices go up later
        }
    }

    return {max_profit, best_buy_index, best_sell_index};
}

vector<double> findTopKDailyGains(const vector<double>& prices, int k)
{
    if (prices.size() < 2 || k <= 0) {
        return {};
    }
    priority_queue<double, vector<double>, greater<double>> min_heap;
    // TODO: Your Logic Here
    // Loop from i=1 to end
    // Calculate gain = prices[i] - prices[i-1]
    // min_heap.push(gain)
    // if min_heap.size() > k -> min_heap.pop()
    double gains=0.0;
    for (int i = 1, len= prices.size(); i < len-1; i++)
    {
        gains=prices[i]-prices[i-1];
        min_heap.push(gains);
        if (min_heap.size() > k)
            min_heap.pop();
    }
    
    vector<double> result;
    while (!min_heap.empty()) {
        result.push_back(min_heap.top());
        min_heap.pop();
    }
    return result;
}

vector<double> calculateEMA(const vector<double>& prices, int period) {
    if (prices.size() < period) return {};

    vector<double> ema_values;
    double multiplier = 2.0 / (period + 1);

    // 1. Initialize First EMA with SMA
    double sum = 0.0;
    for (int i = 0; i < period; ++i) {
        sum += prices[i];
    }
    double prev_ema = sum / period;
    
    // We fill the first 'period-1' slots with 0 or NaN, or just skip them.
    // To keep arrays aligned with prices, let's fill with 0.0 until the period hits.
    for (int i = 0; i < period - 1; ++i) ema_values.push_back(0.0);
    
    ema_values.push_back(prev_ema); // The first valid EMA point

    // 2. Calculate the rest
    for (int i = period; i < prices.size(); ++i) {
        double current_ema = (prices[i] - prev_ema) * multiplier + prev_ema;
        ema_values.push_back(current_ema);
        prev_ema = current_ema;
    }

    return ema_values;
}

std::vector<double> calculateRSI(const std::vector<double>& prices, int period) {
    if (prices.size() <= period) return {};

    std::vector<double> rsi_values;
    // Align result with prices vector (fill initial gap with 0)
    for (int i = 0; i < period; ++i) rsi_values.push_back(0.0);

    double avg_gain = 0.0;
    double avg_loss = 0.0;

    // 1. Initial Average Gain/Loss (Simple Average)
    for (int i = 1; i <= period; ++i) {
        double change = prices[i] - prices[i - 1];
        if (change > 0) avg_gain += change;
        else avg_loss += abs(change);
    }
    avg_gain /= period;
    avg_loss /= period;

    // First RSI point
    double rs = (avg_loss == 0) ? 100.0 : (avg_gain / avg_loss);
    double rsi = 100.0 - (100.0 / (1.0 + rs));
    rsi_values.push_back(rsi);

    // 2. Smoothed Averages (Wilder's Method) for the rest
    for (int i = period + 1; i < prices.size(); ++i) {
        double change = prices[i] - prices[i - 1];
        double current_gain = (change > 0) ? change : 0.0;
        double current_loss = (change < 0) ? abs(change) : 0.0;

        // Previous Avg * (Period - 1) + Current / Period
        avg_gain = ((avg_gain * (period - 1)) + current_gain) / period;
        avg_loss = ((avg_loss * (period - 1)) + current_loss) / period;

        rs = (avg_loss == 0) ? 100.0 : (avg_gain / avg_loss);
        rsi = 100.0 - (100.0 / (1.0 + rs));
        rsi_values.push_back(rsi);
    }

    return rsi_values;
}