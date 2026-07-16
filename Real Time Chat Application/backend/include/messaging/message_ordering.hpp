#pragma once

#include "../common/types.hpp"
#include "../cache/redis_client.hpp"

namespace chat {

class MessageOrdering {
public:
    explicit MessageOrdering(RedisClient& redis);

    int64_t next_sequence(const ConversationID& conv_id);

private:
    RedisClient& redis_;
};

} // namespace chat
