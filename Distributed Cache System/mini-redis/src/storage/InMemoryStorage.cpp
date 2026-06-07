#include "storage/InMemoryStorage.h"

void InMemoryStorage::set(const std::string& key, const std::string& value) {
    store[key] = value;
}

std::optional<std::string> InMemoryStorage::get(const std::string& key) const {
    auto it = store.find(key);
    if (it != store.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool InMemoryStorage::remove(const std::string& key) {
    return store.erase(key) > 0;
}

bool InMemoryStorage::exists(const std::string& key) const {
    return store.find(key) != store.end();
}

void InMemoryStorage::clear() {
    store.clear();
}

size_t InMemoryStorage::size() const {
    return store.size();
}

std::vector<std::string> InMemoryStorage::getAllKeys() const {
    std::vector<std::string> keys;
    keys.reserve(store.size());
    
    for (const auto& pair : store) {
        keys.push_back(pair.first);
    }
    
    return keys;
}
