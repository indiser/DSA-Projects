#pragma once

#include "../common/types.hpp"
#include "../cache/redis_client.hpp"

namespace chat {

class SessionManager {
public:
    explicit SessionManager(RedisClient& redis);

    SessionToken    create_session(const UserID& user_id);
    Result<UserID>  get_user(const SessionToken& token);
    Result<void>    invalidate(const SessionToken& token);
    Result<void>    refresh(const SessionToken& token);

private:
    RedisClient& redis_;
};

} // namespace chat
