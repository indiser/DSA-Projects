#ifndef LFU_POLICY_H
#define LFU_POLICY_H

#include "EvictionPolicy.h"
#include <unordered_map>
#include <map>
#include <list>
#include <string>

class LFUPolicy : public EvictionPolicy {
private:
    struct Node {
        std::string key;
        int frequency;
        std::list<std::string>::iterator listIt;
        
        Node() : key(""), frequency(1) {}
        Node(const std::string& k) : key(k), frequency(1) {}
    };
    
    size_t capacity;
    int minFreq;
    std::unordered_map<std::string, Node> cache;
    std::map<int, std::list<std::string>> freqMap;
    
    void updateFrequency(const std::string& key);
    
public:
    explicit LFUPolicy(size_t capacity);
    ~LFUPolicy() = default;
    
    void onAccess(const std::string& key) override;
    void onInsert(const std::string& key) override;
    std::string evict() override;
    void remove(const std::string& key) override;
};

#endif // LFU_POLICY_H
