#include "cache/LFUPolicy.h"

LFUPolicy::LFUPolicy(size_t capacity) : capacity(capacity), minFreq(0) {}

void LFUPolicy::updateFrequency(const std::string& key) {
    Node& node = cache[key];
    int freq = node.frequency;
    
    // Remove from current frequency list
    freqMap[freq].erase(node.listIt);
    
    // If this was the only key with minFreq and we removed it, update minFreq
    if (freqMap[freq].empty()) {
        freqMap.erase(freq);
        if (minFreq == freq) {
            minFreq++;
        }
    }
    
    // Increment frequency
    node.frequency++;
    freqMap[node.frequency].push_back(key);
    node.listIt = --freqMap[node.frequency].end();
}

void LFUPolicy::onAccess(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        updateFrequency(key);
    }
}

void LFUPolicy::onInsert(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        updateFrequency(key);
    } else {
        Node node(key);
        node.frequency = 1;
        freqMap[1].push_back(key);
        node.listIt = --freqMap[1].end();
        cache[key] = node;
        minFreq = 1;
    }
}

std::string LFUPolicy::evict() {
    if (cache.empty()) {
        return "";
    }
    
    // Get the least frequent key (first in the minFreq list - LRU among same frequency)
    std::string key = freqMap[minFreq].front();
    
    // Remove from frequency list
    freqMap[minFreq].pop_front();
    if (freqMap[minFreq].empty()) {
        freqMap.erase(minFreq);
    }
    
    // Remove from cache
    cache.erase(key);
    
    return key;
}

void LFUPolicy::remove(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        Node& node = cache[key];
        int freq = node.frequency;
        
        // Remove from frequency list
        freqMap[freq].erase(node.listIt);
        if (freqMap[freq].empty()) {
            freqMap.erase(freq);
        }
        
        // Remove from cache
        cache.erase(key);
    }
}
