#include "groups/permissions.hpp"
#include <algorithm>

namespace chat {

bool Permissions::can_add_member(const UserID& requester, const Group& group) {
    return requester == group.owner_id;
}

bool Permissions::can_remove_member(const UserID& requester, const UserID& target,
                                     const Group& group) {
    if (requester == group.owner_id) return true;
    return requester == target; // members can remove themselves
}

bool Permissions::can_delete_group(const UserID& requester, const Group& group) {
    return requester == group.owner_id;
}

} // namespace chat
