#include "notifications/notification_service.hpp"
#include "notifications/push_manager.hpp"

namespace chat {

NotificationService::NotificationService(PushManager& push) : push_(push) {}

void NotificationService::send_push(const UserID& user_id, const Message& message) {
    push_.send(user_id, "New message", message.content);
}

} // namespace chat
