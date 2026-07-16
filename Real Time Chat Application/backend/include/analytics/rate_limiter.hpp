#pragma once

#include "../common/types.hpp"
#include "../cache/redis_client.hpp"
#include <string>

namespace chat {

class RateLimiter {
public:
    explicit RateLimiter(RedisClient& redis);

    bool check_and_increment(const UserID& user_id, const std::string& action,
                             int limit, int window_seconds);

private:
    RedisClient& redis_;
};

} // namespace chat
