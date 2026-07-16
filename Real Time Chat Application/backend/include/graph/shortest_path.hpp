#pragma once

#include "../common/types.hpp"
#include "friendship_graph.hpp"
#include <vector>
#include <optional>

namespace chat {

class ShortestPath {
public:
    explicit ShortestPath(const FriendshipGraph& graph);

    std::optional<std::vector<UserID>> find(const UserID& from, const UserID& to) const;

private:
    const FriendshipGraph& graph_;
};

} // namespace chat
