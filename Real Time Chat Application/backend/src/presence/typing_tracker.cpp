#include "presence/typing_tracker.hpp"
#include "websocket/connection_manager.hpp"
#include "common/constants.hpp"
#include <nlohmann/json.hpp>

namespace chat {

TypingTracker::TypingTracker(RedisClient& redis, ConnectionManager& connections)
    : redis_(redis), connections_(connections) {}

void TypingTracker::start_typing(const UserID& user_id, const ConversationID& conv_id) {
    redis_.set("typing:" + conv_id + ":" + user_id, "1", constants::TYPING_TTL_SECONDS);
    nlohmann::json payload = {{"type", "typing"}, {"user_id", user_id},
                               {"conv_id", conv_id}, {"typing", true}};
    connections_.send_to_conversation(conv_id, payload.dump(), user_id);
}

void TypingTracker::stop_typing(const UserID& user_id, const ConversationID& conv_id) {
    redis_.del("typing:" + conv_id + ":" + user_id);
    nlohmann::json payload = {{"type", "typing"}, {"user_id", user_id},
                               {"conv_id", conv_id}, {"typing", false}};
    connections_.send_to_conversation(conv_id, payload.dump(), user_id);
}

} // namespace chat
