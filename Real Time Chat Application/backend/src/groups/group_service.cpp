#include "groups/group_service.hpp"
#include "groups/permissions.hpp"

namespace chat {

GroupService::GroupService(GroupRepository& groups) : groups_(groups) {}

Result<Group> GroupService::create_group(const UserID& owner_id, const std::string& name) {
    if (name.empty() || name.size() > 100)
        return Result<Group>::fail(ErrorCode::INVALID_INPUT, "invalid group name");
    auto res = groups_.create(name, owner_id);
    if (!res.ok) return res;
    groups_.add_member(res.value.id, owner_id);
    res.value.members.push_back(owner_id);
    return res;
}

Result<void> GroupService::add_member(const GroupID& group_id, const UserID& requester_id,
                                       const UserID& target_user_id) {
    auto res = groups_.find_by_id(group_id);
    if (!res.ok) return Result<void>::fail(res.error, res.message);
    if (!Permissions::can_add_member(requester_id, res.value))
        return Result<void>::fail(ErrorCode::FORBIDDEN, "only owner can add members");
    return groups_.add_member(group_id, target_user_id);
}

Result<void> GroupService::remove_member(const GroupID& group_id, const UserID& requester_id,
                                          const UserID& target_user_id) {
    auto res = groups_.find_by_id(group_id);
    if (!res.ok) return Result<void>::fail(res.error, res.message);
    if (!Permissions::can_remove_member(requester_id, target_user_id, res.value))
        return Result<void>::fail(ErrorCode::FORBIDDEN, "insufficient permissions");
    return groups_.remove_member(group_id, target_user_id);
}

Result<Group> GroupService::get_group(const GroupID& group_id, const UserID& requester_id) {
    auto res = groups_.find_by_id(group_id);
    if (!res.ok) return res;
    bool is_member = false;
    for (const auto& uid : res.value.members)
        if (uid == requester_id) { is_member = true; break; }
    if (!is_member)
        return Result<Group>::fail(ErrorCode::FORBIDDEN, "not a member");
    return res;
}

} // namespace chat
