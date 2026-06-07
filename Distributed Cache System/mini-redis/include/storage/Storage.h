#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <optional>
#include <vector>

class Storage {
public:
    virtual ~Storage() = default;
    
    virtual void set(const std::string& key, const std::string& value) = 0;
    virtual std::optional<std::string> get(const std::string& key) const = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual bool exists(const std::string& key) const = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual std::vector<std::string> getAllKeys() const = 0;
};

#endif // STORAGE_H
