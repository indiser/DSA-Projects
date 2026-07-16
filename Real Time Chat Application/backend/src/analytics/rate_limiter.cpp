#include "analytics/rate_limiter.hpp"

namespace chat {

RateLimiter::RateLimiter(RedisClient& redis) : redis_(redis) {}

bool RateLimiter::check_and_increment(const UserID& user_id, const std::string& action,
                                       int limit, int window_seconds) {
    std::string key = "rl:" + action + ":" + user_id;
    auto val = redis_.get(key);
    int count = val ? std::stoi(*val) : 0;
    if (count >= limit) return false;
    redis_.set(key, std::to_string(count + 1), window_seconds);
    return true;
}

} // namespace chat
