#include "search/trie.hpp"

namespace chat {

void Trie::insert(const std::string& word) {
    Node* cur = &root_;
    for (char c : word) {
        if (!cur->children.count(c))
            cur->children[c] = std::make_unique<Node>();
        cur = cur->children[c].get();
    }
    cur->is_end = true;
}

void Trie::remove(const std::string& word) {
    Node* cur = &root_;
    for (char c : word) {
        auto it = cur->children.find(c);
        if (it == cur->children.end()) return;
        cur = it->second.get();
    }
    cur->is_end = false;
}

std::vector<std::string> Trie::search_prefix(const std::string& prefix, int limit) const {
    const Node* cur = &root_;
    for (char c : prefix) {
        auto it = cur->children.find(c);
        if (it == cur->children.end()) return {};
        cur = it->second.get();
    }
    std::vector<std::string> results;
    std::string current = prefix;
    collect(cur, current, results, limit);
    return results;
}

void Trie::collect(const Node* node, std::string& current,
                   std::vector<std::string>& results, int limit) const {
    if (static_cast<int>(results.size()) >= limit) return;
    if (node->is_end) results.push_back(current);
    for (const auto& [c, child] : node->children) {
        current.push_back(c);
        collect(child.get(), current, results, limit);
        current.pop_back();
    }
}

} // namespace chat
