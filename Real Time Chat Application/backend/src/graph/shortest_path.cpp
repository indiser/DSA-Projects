#include "graph/shortest_path.hpp"
#include <queue>
#include <unordered_map>

namespace chat {

ShortestPath::ShortestPath(const FriendshipGraph& graph) : graph_(graph) {}

std::optional<std::vector<UserID>> ShortestPath::find(const UserID& from,
                                                        const UserID& to) const {
    if (from == to) return std::vector<UserID>{from};

    std::unordered_map<UserID, UserID> parent;
    std::queue<UserID> q;
    q.push(from);
    parent[from] = "";

    while (!q.empty()) {
        UserID cur = q.front(); q.pop();
        for (const auto& nb : graph_.neighbors(cur)) {
            if (parent.count(nb)) continue;
            parent[nb] = cur;
            if (nb == to) {
                std::vector<UserID> path;
                for (UserID n = to; !n.empty(); n = parent[n])
                    path.push_back(n);
                std::reverse(path.begin(), path.end());
                return path;
            }
            q.push(nb);
        }
    }
    return std::nullopt;
}

} // namespace chat
