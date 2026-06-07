#ifndef IN_MEMORY_STORAGE_H
#define IN_MEMORY_STORAGE_H

#include "Storage.h"
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>

class InMemoryStorage : public Storage {
private:
    std::unordered_map<std::string, std::string> store;
    
public:
    InMemoryStorage() = default;
    ~InMemoryStorage() = default;
    
    void set(const std::string& key, const std::string& value) override;
    std::optional<std::string> get(const std::string& key) const override;
    bool remove(const std::string& key) override;
    bool exists(const std::string& key) const override;
    void clear() override;
    size_t size() const override;
    std::vector<std::string> getAllKeys() const override;
};

#endif // IN_MEMORY_STORAGE_H
