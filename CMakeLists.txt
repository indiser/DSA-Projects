cmake_minimum_required(VERSION 3.15)
project(StockAnalysis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/pybind11)

find_package(Python 3.8 COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Found Python executable: ${Python_EXECUTABLE}")
message(STATUS "Found Python includes: ${Python_INCLUDE_DIRS}")

pybind11_add_module(stock_analyzer
    src/analysis.cpp
    src/bindings.cpp
)

target_link_libraries(stock_analyzer PRIVATE Python::Python)

target_include_directories(stock_analyzer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Python_INCLUDE_DIRS}
)