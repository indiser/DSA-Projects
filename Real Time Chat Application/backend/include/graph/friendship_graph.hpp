#pragma once

#include "../common/types.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace chat {

class FriendshipGraph {
public:
    void                 add_edge(const UserID& a, const UserID& b);
    void                 remove_edge(const UserID& a, const UserID& b);
    std::vector<UserID>  neighbors(const UserID& user_id) const;
    bool                 are_connected(const UserID& a, const UserID& b) const;

private:
    std::unordered_map<UserID, std::unordered_set<UserID>> adj_;
};

} // namespace chat
