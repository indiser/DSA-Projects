#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace chat {

class Trie {
public:
    void                     insert(const std::string& word);
    void                     remove(const std::string& word);
    std::vector<std::string> search_prefix(const std::string& prefix, int limit = 10) const;

private:
    struct Node {
        std::unordered_map<char, std::unique_ptr<Node>> children;
        bool is_end = false;
    };

    void collect(const Node* node, std::string& current,
                 std::vector<std::string>& results, int limit) const;

    Node root_;
};

} // namespace chat
