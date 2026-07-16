#pragma once

#include "../common/types.hpp"
#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace chat {

class WebSocketSession;

class ConnectionManager {
public:
    void register_session(const UserID& user_id, std::weak_ptr<WebSocketSession> session);
    void remove_session(const UserID& user_id);
    void send_to_user(const UserID& user_id, const std::string& payload);
    void send_to_conversation(const ConversationID& conv_id, const std::string& payload,
                              const UserID& exclude_user_id = {});

private:
    std::unordered_map<UserID, std::weak_ptr<WebSocketSession>> sessions_;
    std::mutex mu_;
};

} // namespace chat
