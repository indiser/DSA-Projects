#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "Graph.cpp"
#include<Windows.h>
void test_add_edge_bidirectional() {
    Graph graph;
    graph.add_edge(10, 20);

    auto friends_of_10 = graph.get_friends(10);
    assert(friends_of_10.size() == 1);
    assert(friends_of_10[0] == 20);

    auto friends_of_20 = graph.get_friends(20);
    assert(friends_of_20.size() == 1);
    assert(friends_of_20[0] == 10);
    
    std::cout << "✓ AddEdgeIsBidirectional test passed" << std::endl;
}

void test_graph_basic_functionality() {
    Graph g;
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    
    assert(g.user_exists(0));
    assert(g.user_exists(1));
    assert(g.user_exists(2));
    assert(g.user_exists(3));
    assert(!g.user_exists(99));
    
    auto friends_of_1 = g.get_friends(1);
    assert(friends_of_1.size() == 3); // 0, 2, 3
    
    std::cout << "✓ GraphBasicFunctionality test passed" << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);
    std::cout << "Running basic tests..." << std::endl;
    
    test_add_edge_bidirectional();
    test_graph_basic_functionality();
    
    std::cout << "All basic tests passed!" << std::endl;
    return 0;
}