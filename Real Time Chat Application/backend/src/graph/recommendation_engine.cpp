#include "graph/recommendation_engine.hpp"
#include <unordered_map>
#include <algorithm>

namespace chat {

RecommendationEngine::RecommendationEngine(const FriendshipGraph& graph) : graph_(graph) {}

std::vector<UserID> RecommendationEngine::suggest_friends(const UserID& user_id,
                                                           int limit) const {
    auto direct = graph_.neighbors(user_id);
    std::unordered_set<UserID> direct_set(direct.begin(), direct.end());
    direct_set.insert(user_id);

    std::unordered_map<UserID, int> score;
    for (const auto& friend_id : direct) {
        for (const auto& fof : graph_.neighbors(friend_id)) {
            if (!direct_set.count(fof))
                ++score[fof];
        }
    }

    std::vector<std::pair<int, UserID>> ranked;
    ranked.reserve(score.size());
    for (const auto& [uid, s] : score)
        ranked.push_back({s, uid});

    std::sort(ranked.begin(), ranked.end(), std::greater<>());

    std::vector<UserID> result;
    for (int i = 0; i < std::min(limit, static_cast<int>(ranked.size())); ++i)
        result.push_back(ranked[i].second);
    return result;
}

} // namespace chat
