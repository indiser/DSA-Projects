#include "cache/CacheEngine.h"
#include "cache/LRUPolicy.h"
#include "cache/LFUPolicy.h"
#include "storage/InMemoryStorage.h"
#include <mutex>

CacheEngine::CacheEngine(size_t capacity, EvictionPolicyType policy) 
    : capacity(capacity) {
    
    stats.hits = 0;
    stats.misses = 0;
    stats.size = 0;
    
    // Create eviction policy
    if (policy == EvictionPolicyType::LRU) {
        evictionPolicy = std::make_unique<LRUPolicy>(capacity);
    } else {
        evictionPolicy = std::make_unique<LFUPolicy>(capacity);
    }
    
    // Create storage
    storage = std::make_unique<InMemoryStorage>();
}

bool CacheEngine::isExpired(const std::string& key) {
    if (ttlMap.find(key) == ttlMap.end()) {
        return false;
    }
    
    auto now = std::chrono::steady_clock::now();
    return now >= ttlMap[key].expireTime;
}

void CacheEngine::cleanupExpired(const std::string& key) {
    if (isExpired(key)) {
        storage->remove(key);
        evictionPolicy->remove(key);
        ttlMap.erase(key);
    }
}

std::optional<std::string> CacheEngine::get(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    
    cleanupExpired(key);
    
    auto value = storage->get(key);
    
    if (value.has_value()) {
        stats.hits++;
        evictionPolicy->onAccess(key);
        return value;
    }
    
    stats.misses++;
    return std::nullopt;
}

void CacheEngine::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    
    // If key exists, update it
    if (storage->exists(key)) {
        storage->set(key, value);
        evictionPolicy->onAccess(key);
        return;
    }
    
    // If at capacity, evict
    if (storage->size() >= capacity) {
        std::string evictedKey = evictionPolicy->evict();
        if (!evictedKey.empty()) {
            storage->remove(evictedKey);
            ttlMap.erase(evictedKey);
        }
    }
    
    // Insert new key
    storage->set(key, value);
    evictionPolicy->onInsert(key);
}

bool CacheEngine::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    
    if (storage->remove(key)) {
        evictionPolicy->remove(key);
        ttlMap.erase(key);
        return true;
    }
    
    return false;
}

bool CacheEngine::exists(const std::string& key) {
    std::shared_lock<std::shared_mutex> lock(mutex);
    
    if (isExpired(key)) {
        return false;
    }
    
    return storage->exists(key);
}

void CacheEngine::setTTL(const std::string& key, int seconds) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    
    if (storage->exists(key)) {
        TTLInfo info;
        info.expireTime = std::chrono::steady_clock::now() + 
                         std::chrono::seconds(seconds);
        ttlMap[key] = info;
    }
}

void CacheEngine::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex);
    
    auto keys = storage->getAllKeys();
    for (const auto& key : keys) {
        evictionPolicy->remove(key);
    }
    
    storage->clear();
    ttlMap.clear();
}

size_t CacheEngine::size() const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return storage->size();
}

CacheStats CacheEngine::getStats() const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    CacheStats currentStats = stats;
    currentStats.size = storage->size();
    return currentStats;
}

std::vector<std::string> CacheEngine::getAllKeys() const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return storage->getAllKeys();
}
