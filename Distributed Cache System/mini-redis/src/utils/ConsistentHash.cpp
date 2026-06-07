#include "utils/ConsistentHash.h"
#include <set>

ConsistentHash::ConsistentHash(int virtualNodes) : virtualNodes(virtualNodes) {}

size_t ConsistentHash::hash(const std::string& key) const {
    return hasher(key);
}

void ConsistentHash::addNode(const std::string& node) {
    for (int i = 0; i < virtualNodes; i++) {
        std::string virtualKey = node + "#" + std::to_string(i);
        size_t hashValue = hash(virtualKey);
        ring[hashValue] = node;
    }
}

void ConsistentHash::removeNode(const std::string& node) {
    for (int i = 0; i < virtualNodes; i++) {
        std::string virtualKey = node + "#" + std::to_string(i);
        size_t hashValue = hash(virtualKey);
        ring.erase(hashValue);
    }
}

std::string ConsistentHash::getNode(const std::string& key) const {
    if (ring.empty()) {
        return "";
    }
    
    size_t hashValue = hash(key);
    
    // Find the first node with hash >= key's hash (clockwise on ring)
    auto it = ring.lower_bound(hashValue);
    
    // If no node found, wrap around to the first node
    if (it == ring.end()) {
        it = ring.begin();
    }
    
    return it->second;
}

size_t ConsistentHash::getNodeCount() const {
    // Count unique nodes
    std::set<std::string> uniqueNodes;
    for (const auto& pair : ring) {
        uniqueNodes.insert(pair.second);
    }
    return uniqueNodes.size();
}
