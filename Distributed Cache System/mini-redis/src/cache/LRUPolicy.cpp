#include "cache/LRUPolicy.h"

LRUPolicy::LRUPolicy(size_t capacity) : capacity(capacity) {
    head = new Node("");
    tail = new Node("");
    head->next = tail;
    tail->prev = head;
}

LRUPolicy::~LRUPolicy() {
    for (auto& pair : cache) {
        delete pair.second;
    }
    delete head;
    delete tail;
}

void LRUPolicy::moveToFront(Node* node) {
    removeNode(node);
    addToFront(node);
}

void LRUPolicy::addToFront(Node* node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

void LRUPolicy::removeNode(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void LRUPolicy::onAccess(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        moveToFront(cache[key]);
    }
}

void LRUPolicy::onInsert(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        moveToFront(cache[key]);
    } else {
        Node* node = new Node(key);
        cache[key] = node;
        addToFront(node);
    }
}

std::string LRUPolicy::evict() {
    if (cache.empty()) {
        return "";
    }
    
    Node* lru = tail->prev;
    if (lru == head) {
        return "";
    }
    
    std::string key = lru->key;
    removeNode(lru);
    cache.erase(key);
    delete lru;
    
    return key;
}

void LRUPolicy::remove(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        Node* node = cache[key];
        removeNode(node);
        cache.erase(key);
        delete node;
    }
}
