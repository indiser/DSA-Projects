#include<iostream>
#include<vector>
#include<unordered_map>
#include "Graph.h"
using namespace std;

void Graph::add_edge(int u, int v) {
    // Your implementation logic goes here
    adj[u].push_back(v);
    adj[v].push_back(u);
}

const vector<int>& Graph::get_friends(int u) const {
    // Your implementation logic goes here
    // You'll need to handle returning a vector.
    return adj.at(u);
}

bool Graph::user_exists(int u) const { return adj.count(u) > 0; }