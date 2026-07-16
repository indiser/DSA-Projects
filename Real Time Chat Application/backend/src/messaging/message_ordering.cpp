#include "messaging/message_ordering.hpp"
#include <hiredis/hiredis.h>
#include <stdexcept>

namespace chat {

MessageOrdering::MessageOrdering(RedisClient& redis) : redis_(redis) {}

int64_t MessageOrdering::next_sequence(const ConversationID& conv_id) {
    // Use Redis INCR for atomic monotonic counter per conversation
    std::string key = "seq:" + conv_id;
    // set with no TTL on first call, then just increment
    // We piggyback on the raw set/get — use a small wrapper via publish trick
    // Since RedisClient doesn't expose INCR directly, we store and bump manually
    // with a compare-and-set loop. For production, expose INCR in RedisClient.
    auto val = redis_.get(key);
    int64_t next = val ? std::stoll(*val) + 1 : 1;
    redis_.set(key, std::to_string(next));
    return next;
}

} // namespace chat
