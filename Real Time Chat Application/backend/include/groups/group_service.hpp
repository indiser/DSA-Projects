#pragma once

#include "../common/types.hpp"
#include "../database/repositories/group_repository.hpp"

namespace chat {

class GroupService {
public:
    explicit GroupService(GroupRepository& groups);

    Result<Group> create_group(const UserID& owner_id, const std::string& name);
    Result<void>  add_member(const GroupID& group_id, const UserID& requester_id, const UserID& target_user_id);
    Result<void>  remove_member(const GroupID& group_id, const UserID& requester_id, const UserID& target_user_id);
    Result<Group> get_group(const GroupID& group_id, const UserID& requester_id);

private:
    GroupRepository& groups_;
};

} // namespace chat
