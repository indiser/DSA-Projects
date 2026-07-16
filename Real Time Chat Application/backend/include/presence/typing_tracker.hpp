#pragma once

#include "../common/types.hpp"
#include "../cache/redis_client.hpp"

namespace chat {

class ConnectionManager;

class TypingTracker {
public:
    TypingTracker(RedisClient& redis, ConnectionManager& connections);

    void start_typing(const UserID& user_id, const ConversationID& conv_id);
    void stop_typing(const UserID& user_id, const ConversationID& conv_id);

private:
    RedisClient&       redis_;
    ConnectionManager& connections_;
};

} // namespace chat
