#pragma once

#include "../common/types.hpp"
#include "../database/repositories/messege_repository.hpp"

namespace chat {

class DeliveryManager {
public:
    explicit DeliveryManager(MessageRepository& messages);

    Result<void> acknowledge(const MessageID& message_id);
    Result<void> mark_read(const MessageID& message_id);

private:
    MessageRepository& messages_;
};

} // namespace chat
