#include "presence/presence_service.hpp"
#include "common/constants.hpp"

namespace chat {

PresenceService::PresenceService(RedisClient& redis) : redis_(redis) {}

void PresenceService::user_connected(const UserID& user_id) {
    redis_.set("online:" + user_id, "1", constants::PRESENCE_TTL_SECONDS);
}

void PresenceService::user_disconnected(const UserID& user_id) {
    redis_.del("online:" + user_id);
}

bool PresenceService::is_online(const UserID& user_id) {
    return redis_.get("online:" + user_id).has_value();
}

std::vector<UserID> PresenceService::get_online_users(const std::vector<UserID>& user_ids) {
    std::vector<UserID> online;
    for (const auto& uid : user_ids)
        if (is_online(uid)) online.push_back(uid);
    return online;
}

} // namespace chat
