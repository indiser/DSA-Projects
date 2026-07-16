#include "websocket/connection_manager.hpp"
#include "websocket/websocket_session.hpp"

namespace chat {

void ConnectionManager::register_session(const UserID& user_id,
                                          std::weak_ptr<WebSocketSession> session) {
    std::lock_guard<std::mutex> lock(mu_);
    sessions_[user_id] = std::move(session);
}

void ConnectionManager::remove_session(const UserID& user_id) {
    std::lock_guard<std::mutex> lock(mu_);
    sessions_.erase(user_id);
}

void ConnectionManager::send_to_user(const UserID& user_id, const std::string& payload) {
    std::lock_guard<std::mutex> lock(mu_);
    auto it = sessions_.find(user_id);
    if (it == sessions_.end()) return;
    if (auto session = it->second.lock())
        session->send(payload);
    else
        sessions_.erase(it);
}

void ConnectionManager::send_to_conversation(const ConversationID& conv_id,
                                              const std::string& payload,
                                              const UserID& exclude_user_id) {
    std::lock_guard<std::mutex> lock(mu_);
    for (auto it = sessions_.begin(); it != sessions_.end(); ) {
        if (it->first == exclude_user_id) { ++it; continue; }
        if (auto session = it->second.lock()) {
            session->send(payload);
            ++it;
        } else {
            it = sessions_.erase(it);
        }
    }
}

} // namespace chat
