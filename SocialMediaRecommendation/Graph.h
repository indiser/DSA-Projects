#pragma once // Prevents multiple inclusions

#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

class Graph {
private:
    unordered_map<int, vector<int>> adj;

public:
    void add_edge(int u, int v);
    const vector<int>& get_friends(int u) const;
    bool user_exists(int u) const;
};