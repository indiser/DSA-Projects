#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace chat {

class InvertedIndex {
public:
    void                     insert(const std::string& username);
    void                     remove(const std::string& username);
    std::vector<std::string> lookup(const std::string& keyword) const;

private:
    // token -> set of usernames containing that token
    std::unordered_map<std::string, std::unordered_set<std::string>> index_;
};

} // namespace chat
