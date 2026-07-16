#pragma once

#include "../common/types.hpp"

namespace chat {

class ConnectionManager;
class PresenceService;
class NotificationService;

class MessageDispatcher {
public:
    MessageDispatcher(ConnectionManager& connections, PresenceService& presence,
                      NotificationService& notifications);

    void dispatch(const Message& message);

private:
    ConnectionManager&   connections_;
    PresenceService&     presence_;
    NotificationService& notifications_;
};

} // namespace chat
