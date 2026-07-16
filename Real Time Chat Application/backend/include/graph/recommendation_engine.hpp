#pragma once

#include "../common/types.hpp"
#include "friendship_graph.hpp"
#include <vector>

namespace chat {

class RecommendationEngine {
public:
    explicit RecommendationEngine(const FriendshipGraph& graph);

    std::vector<UserID> suggest_friends(const UserID& user_id, int limit = 10) const;

private:
    const FriendshipGraph& graph_;
};

} // namespace chat
