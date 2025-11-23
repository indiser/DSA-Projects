#pragma once
#include <vector>
using namespace std;

struct TradeResult {
    double max_profit;
    int buy_day_index;
    int sell_day_index;
};

int add(int i, int j);
double calculateSMA(const vector<double>& prices, int window);
TradeResult findMaxProfit(const vector<double>& prices);
vector<double> findTopKDailyGains(const vector<double>& prices, int k);
vector<double> calculateEMA(const vector<double>& prices, int period);
vector<double> calculateRSI(const vector<double>& prices, int period);