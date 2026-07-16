#pragma once

#include "../common/types.hpp"

namespace chat {

class Permissions {
public:
    static bool can_add_member(const UserID& requester, const Group& group);
    static bool can_remove_member(const UserID& requester, const UserID& target, const Group& group);
    static bool can_delete_group(const UserID& requester, const Group& group);
};

} // namespace chat
