#include "graph/friendship_graph.hpp"

namespace chat {

void FriendshipGraph::add_edge(const UserID& a, const UserID& b) {
    adj_[a].insert(b);
    adj_[b].insert(a);
}

void FriendshipGraph::remove_edge(const UserID& a, const UserID& b) {
    adj_[a].erase(b);
    adj_[b].erase(a);
}

std::vector<UserID> FriendshipGraph::neighbors(const UserID& user_id) const {
    auto it = adj_.find(user_id);
    if (it == adj_.end()) return {};
    return std::vector<UserID>(it->second.begin(), it->second.end());
}

bool FriendshipGraph::are_connected(const UserID& a, const UserID& b) const {
    auto it = adj_.find(a);
    if (it == adj_.end()) return false;
    return it->second.count(b) > 0;
}

} // namespace chat
