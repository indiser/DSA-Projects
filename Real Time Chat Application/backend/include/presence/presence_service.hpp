#pragma once

#include "../common/types.hpp"
#include "../cache/redis_client.hpp"
#include <vector>

namespace chat {

class PresenceService {
public:
    explicit PresenceService(RedisClient& redis);

    void                 user_connected(const UserID& user_id);
    void                 user_disconnected(const UserID& user_id);
    bool                 is_online(const UserID& user_id);
    std::vector<UserID>  get_online_users(const std::vector<UserID>& user_ids);

private:
    RedisClient& redis_;
};

} // namespace chat
