#ifndef EVICTION_POLICY_H
#define EVICTION_POLICY_H

#include <string>

class EvictionPolicy {
public:
    virtual ~EvictionPolicy() = default;
    
    virtual void onAccess(const std::string& key) = 0;
    virtual void onInsert(const std::string& key) = 0;
    virtual std::string evict() = 0;
    virtual void remove(const std::string& key) = 0;
};

#endif // EVICTION_POLICY_H
