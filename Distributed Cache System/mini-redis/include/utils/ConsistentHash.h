#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include <string>
#include <map>
#include <functional>

class ConsistentHash {
private:
    std::map<size_t, std::string> ring;
    int virtualNodes;
    std::hash<std::string> hasher;
    
    size_t hash(const std::string& key) const;
    
public:
    explicit ConsistentHash(int virtualNodes = 150);
    ~ConsistentHash() = default;
    
    void addNode(const std::string& node);
    void removeNode(const std::string& node);
    std::string getNode(const std::string& key) const;
    
    size_t getNodeCount() const;
};

#endif // CONSISTENT_HASH_H
