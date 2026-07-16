#include "messaging/delivery_manager.hpp"

namespace chat {

DeliveryManager::DeliveryManager(MessageRepository& messages) : messages_(messages) {}

Result<void> DeliveryManager::acknowledge(const MessageID& message_id) {
    return messages_.mark_delivered(message_id);
}

Result<void> DeliveryManager::mark_read(const MessageID& message_id) {
    return messages_.mark_read(message_id);
}

} // namespace chat
