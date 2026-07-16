#pragma once

#include "redis_client.hpp"
#include "../common/types.hpp"
#include <vector>
#include <optional>

namespace chat {

class CacheManager {
public:
    explicit CacheManager(RedisClient& redis);

    // Sessions
    bool                       set_session(const SessionToken& token, const UserID& user_id);
    std::optional<UserID>      get_session(const SessionToken& token);
    bool                       delete_session(const SessionToken& token);

    // Presence
    bool set_online(const UserID& user_id);
    bool set_offline(const UserID& user_id);
    bool refresh_presence(const UserID& user_id);

    // Messages
    bool                          cache_messages(const ConversationID& conv_id, const std::vector<Message>& messages);
    std::optional<std::vector<Message>> get_cached_messages(const ConversationID& conv_id);
    bool                          invalidate_messages(const ConversationID& conv_id);

    // Typing
    bool set_typing(const UserID& user_id, const ConversationID& conv_id);
    bool clear_typing(const UserID& user_id, const ConversationID& conv_id);

private:
    RedisClient& redis_;
};

} // namespace chat
