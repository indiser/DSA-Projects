#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};

/*
Time Complexity: O(V + E)
- V: number of vertices (nodes) in the graph
- E: number of edges in the graph
- Each node is visited exactly once during DFS traversal
- Each edge is processed once when cloning neighbors
- Hash map operations (find, insert) are O(1) average case
- Total: O(V + E)

Space Complexity: O(V)
- Hash map storage: O(V) to store original->clone mapping
- Recursion stack: O(V) in worst case for deep graphs
- Cloned graph space: O(V + E) but not counted as auxiliary space
- Total auxiliary space: O(V)
*/
class Solution {
public:
    unordered_map<Node*, Node*> visited;
    
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        
        if (visited.find(node) != visited.end()) {
            return visited[node];
        }
        
        Node* clone = new Node(node->val);
        visited[node] = clone;
        
        for (Node* neighbor : node->neighbors) {
            clone->neighbors.push_back(cloneGraph(neighbor));
        }
        
        return clone;
    }
};

// Helper function to print graph
void printGraph(Node* node, unordered_map<Node*, bool>& printed) {
    if (!node || printed[node]) return;
    
    printed[node] = true;
    cout << "Node " << node->val << " -> neighbors: ";
    for (Node* neighbor : node->neighbors) {
        cout << neighbor->val << " ";
    }
    cout << endl;
    
    for (Node* neighbor : node->neighbors) {
        printGraph(neighbor, printed);
    }
}

void printGraph(Node* node) {
    if (!node) {
        cout << "Empty graph" << endl;
        return;
    }
    unordered_map<Node*, bool> printed;
    printGraph(node, printed);
}

int main()
{
    cout << "=== Clone Graph ===" << endl;
    
    Solution sol;
    
    // Test case 1: Simple connected graph
    cout << "\nTest Case 1: Simple connected graph" << endl;
    
    // Create nodes
    Node* node1 = new Node(1);
    Node* node2 = new Node(2);
    Node* node3 = new Node(3);
    Node* node4 = new Node(4);
    
    // Create connections: 1-2, 1-4, 2-3, 3-4 (forming a cycle)
    node1->neighbors = {node2, node4};
    node2->neighbors = {node1, node3};
    node3->neighbors = {node2, node4};
    node4->neighbors = {node1, node3};
    
    cout << "Original graph:" << endl;
    printGraph(node1);
    
    cout << "\nCloning the graph..." << endl;
    Node* cloned1 = sol.cloneGraph(node1);
    
    cout << "\nCloned graph:" << endl;
    printGraph(cloned1);
    
    cout << "\nVerification:" << endl;
    cout << "Original node 1 address: " << node1 << ", Cloned node 1 address: " << cloned1 << endl;
    cout << "Different addresses confirm successful deep copy" << endl;
    
    // Test case 2: Single node
    cout << "\nTest Case 2: Single node graph" << endl;
    Node* single = new Node(42);
    
    cout << "Original graph:" << endl;
    printGraph(single);
    
    Node* clonedSingle = sol.cloneGraph(single);
    
    cout << "\nCloned graph:" << endl;
    printGraph(clonedSingle);
    
    cout << "\nVerification:" << endl;
    cout << "Original: " << single << " (val: " << single->val << ")" << endl;
    cout << "Cloned: " << clonedSingle << " (val: " << clonedSingle->val << ")" << endl;
    
    // Test case 3: Empty graph
    cout << "\nTest Case 3: Empty graph" << endl;
    Node* empty = nullptr;
    
    cout << "Original graph: ";
    printGraph(empty);
    
    Node* clonedEmpty = sol.cloneGraph(empty);
    
    cout << "Cloned graph: ";
    printGraph(clonedEmpty);
    
    cout << "\nAlgorithm: DFS with hash map to track visited nodes" << endl;
    cout << "Creates deep copy while preserving graph structure and connections" << endl;
    cout << "Handles cycles by checking if node already cloned" << endl;
    
    return 0;
}