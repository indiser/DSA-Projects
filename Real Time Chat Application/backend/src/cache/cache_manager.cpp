#include "cache/cache_manager.hpp"
#include "common/constants.hpp"
#include <nlohmann/json.hpp>

namespace chat {

using json = nlohmann::json;

CacheManager::CacheManager(RedisClient& redis) : redis_(redis) {}

// --- Sessions ---

bool CacheManager::set_session(const SessionToken& token, const UserID& user_id) {
    return redis_.set("session:" + token, user_id, constants::SESSION_TTL_SECONDS);
}

std::optional<UserID> CacheManager::get_session(const SessionToken& token) {
    return redis_.get("session:" + token);
}

bool CacheManager::delete_session(const SessionToken& token) {
    return redis_.del("session:" + token);
}

// --- Presence ---

bool CacheManager::set_online(const UserID& user_id) {
    return redis_.set("online:" + user_id, "1", constants::PRESENCE_TTL_SECONDS);
}

bool CacheManager::set_offline(const UserID& user_id) {
    return redis_.del("online:" + user_id);
}

bool CacheManager::refresh_presence(const UserID& user_id) {
    return redis_.expire("online:" + user_id, constants::PRESENCE_TTL_SECONDS);
}

// --- Messages ---

bool CacheManager::cache_messages(const ConversationID& conv_id,
                                   const std::vector<Message>& messages) {
    json arr = json::array();
    for (const auto& m : messages) {
        arr.push_back({
            {"id",              m.id},
            {"conversation_id", m.conversation_id},
            {"sender_id",       m.sender_id},
            {"content",         m.content},
            {"delivered",       m.delivered},
            {"read",            m.read},
            {"timestamp",       m.timestamp}
        });
    }
    return redis_.set("messages:" + conv_id, arr.dump(), 600);
}

std::optional<std::vector<Message>> CacheManager::get_cached_messages(
    const ConversationID& conv_id) {
    auto raw = redis_.get("messages:" + conv_id);
    if (!raw) return std::nullopt;
    try {
        auto arr = json::parse(*raw);
        std::vector<Message> msgs;
        for (const auto& j : arr) {
            Message m;
            m.id              = j["id"];
            m.conversation_id = j["conversation_id"];
            m.sender_id       = j["sender_id"];
            m.content         = j["content"];
            m.delivered       = j["delivered"];
            m.read            = j["read"];
            m.timestamp       = j["timestamp"];
            msgs.push_back(std::move(m));
        }
        return msgs;
    } catch (...) {
        return std::nullopt;
    }
}

bool CacheManager::invalidate_messages(const ConversationID& conv_id) {
    return redis_.del("messages:" + conv_id);
}

// --- Typing ---

bool CacheManager::set_typing(const UserID& user_id, const ConversationID& conv_id) {
    return redis_.set("typing:" + conv_id + ":" + user_id, "1", constants::TYPING_TTL_SECONDS);
}

bool CacheManager::clear_typing(const UserID& user_id, const ConversationID& conv_id) {
    return redis_.del("typing:" + conv_id + ":" + user_id);
}

} // namespace chat
