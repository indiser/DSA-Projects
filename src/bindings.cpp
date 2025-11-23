#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // AUTOMATICALLY converts std::vector to Python lists
#include "analysis.h"

namespace py = pybind11;

PYBIND11_MODULE(stock_analyzer, m) {
    m.doc() = "C++ High Frequency Analysis Engine"; 
    py::class_<TradeResult>(m, "TradeResult")
        .def_readonly("max_profit", &TradeResult::max_profit)
        .def_readonly("buy_day_index", &TradeResult::buy_day_index)
        .def_readonly("sell_day_index", &TradeResult::sell_day_index);

    m.def("add", &add, "A function that adds two numbers");
    m.def("calculate_sma", &calculateSMA, "Calculate Simple Moving Average");
    m.def("find_max_profit", &findMaxProfit, "Returns a TradeResult object");
    m.def("find_top_k_gains", &findTopKDailyGains, "Returns the top K daily gains using a Min-Heap");
    m.def("calculate_ema", &calculateEMA, "Calculate Exponential Moving Average");
    m.def("calculate_rsi", &calculateRSI, "Calculate Relative Strength Index (Wilder's Smoothing)");
}