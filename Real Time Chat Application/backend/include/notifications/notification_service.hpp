#pragma once

#include "../common/types.hpp"

namespace chat {

class PushManager;

class NotificationService {
public:
    explicit NotificationService(PushManager& push);

    void send_push(const UserID& user_id, const Message& message);

private:
    PushManager& push_;
};

} // namespace chat
