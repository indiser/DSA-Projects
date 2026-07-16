#pragma once

#include <unordered_map>
#include <list>
#include <optional>
#include <mutex>

namespace chat {

template<typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(size_t capacity);

    std::optional<V> get(const K& key);
    void             put(const K& key, V value);
    void             evict(const K& key);
    size_t           size() const;

private:
    size_t                                          capacity_;
    std::list<std::pair<K, V>>                      order_;
    std::unordered_map<K, typename std::list<std::pair<K,V>>::iterator> map_;
    mutable std::mutex                              mu_;
};

} // namespace chat
