#ifndef CACHE_ENGINE_H
#define CACHE_ENGINE_H

#include "EvictionPolicy.h"
#include "storage/Storage.h"
#include <memory>
#include <optional>
#include <string>
#include <shared_mutex>
#include <unordered_map>
#include <chrono>

enum class EvictionPolicyType {
    LRU,
    LFU
};

struct CacheStats {
    size_t hits;
    size_t misses;
    size_t size;
    
    double hitRate() const {
        if (hits + misses == 0) return 0.0;
        return (100.0 * hits) / (hits + misses);
    }
};

class CacheEngine {
private:
    std::unique_ptr<EvictionPolicy> evictionPolicy;
    std::unique_ptr<Storage> storage;
    mutable std::shared_mutex mutex;
    size_t capacity;
    
    CacheStats stats;
    
    struct TTLInfo {
        std::chrono::steady_clock::time_point expireTime;
    };
    std::unordered_map<std::string, TTLInfo> ttlMap;
    
    bool isExpired(const std::string& key);
    void cleanupExpired(const std::string& key);
    
public:
    CacheEngine(size_t capacity, EvictionPolicyType policy);
    ~CacheEngine() = default;
    
    std::optional<std::string> get(const std::string& key);
    void put(const std::string& key, const std::string& value);
    bool remove(const std::string& key);
    bool exists(const std::string& key);
    
    void setTTL(const std::string& key, int seconds);
    void clear();
    size_t size() const;
    CacheStats getStats() const;
    std::vector<std::string> getAllKeys() const;
};

#endif // CACHE_ENGINE_H
