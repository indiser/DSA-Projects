#include "search/inverted_index.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace chat {

static std::vector<std::string> tokenize(const std::string& s) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, '_')) {
        std::transform(tok.begin(), tok.end(), tok.begin(), ::tolower);
        if (!tok.empty()) tokens.push_back(tok);
    }
    return tokens;
}

void InvertedIndex::insert(const std::string& username) {
    for (const auto& token : tokenize(username))
        index_[token].insert(username);
}

void InvertedIndex::remove(const std::string& username) {
    for (const auto& token : tokenize(username)) {
        auto it = index_.find(token);
        if (it != index_.end()) it->second.erase(username);
    }
}

std::vector<std::string> InvertedIndex::lookup(const std::string& keyword) const {
    std::string key = keyword;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    auto it = index_.find(key);
    if (it == index_.end()) return {};
    return std::vector<std::string>(it->second.begin(), it->second.end());
}

} // namespace chat
