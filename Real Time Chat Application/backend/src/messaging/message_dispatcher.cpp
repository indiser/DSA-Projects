#include "messaging/message_dispatcher.hpp"
#include "websocket/connection_manager.hpp"
#include "presence/presence_service.hpp"
#include "notifications/notification_service.hpp"
#include <nlohmann/json.hpp>

namespace chat {

MessageDispatcher::MessageDispatcher(ConnectionManager& connections,
                                     PresenceService& presence,
                                     NotificationService& notifications)
    : connections_(connections), presence_(presence), notifications_(notifications) {}

void MessageDispatcher::dispatch(const Message& message) {
    nlohmann::json payload = {
        {"type",            "message"},
        {"message_id",      message.id},
        {"conversation_id", message.conversation_id},
        {"sender_id",       message.sender_id},
        {"content",         message.content},
        {"timestamp",       message.timestamp}
    };
    std::string raw = payload.dump();

    // Send to all participants in the conversation except the sender
    connections_.send_to_conversation(message.conversation_id, raw, message.sender_id);
}

} // namespace chat
