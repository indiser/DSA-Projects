#pragma once

#include "../common/types.hpp"
#include "presence_service.hpp"

namespace chat {

class OnlineTracker {
public:
    explicit OnlineTracker(PresenceService& presence);

    void on_ping(const UserID& user_id);

private:
    PresenceService& presence_;
};

} // namespace chat
