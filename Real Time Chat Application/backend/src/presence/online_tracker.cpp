#include "presence/online_tracker.hpp"

namespace chat {

OnlineTracker::OnlineTracker(PresenceService& presence) : presence_(presence) {}

void OnlineTracker::on_ping(const UserID& user_id) {
    presence_.user_connected(user_id); // refreshes TTL
}

} // namespace chat
