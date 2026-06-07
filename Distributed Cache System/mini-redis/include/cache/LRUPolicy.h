#ifndef LRU_POLICY_H
#define LRU_POLICY_H

#include "EvictionPolicy.h"
#include <unordered_map>
#include <string>

class LRUPolicy : public EvictionPolicy {
private:
    struct Node {
        std::string key;
        Node* prev;
        Node* next;
        
        Node(const std::string& k) : key(k), prev(nullptr), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    std::unordered_map<std::string, Node*> cache;
    size_t capacity;
    
    void moveToFront(Node* node);
    void addToFront(Node* node);
    void removeNode(Node* node);
    
public:
    explicit LRUPolicy(size_t capacity);
    ~LRUPolicy();
    
    void onAccess(const std::string& key) override;
    void onInsert(const std::string& key) override;
    std::string evict() override;
    void remove(const std::string& key) override;
};

#endif // LRU_POLICY_H
