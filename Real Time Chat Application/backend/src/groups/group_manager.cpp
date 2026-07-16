#include "groups/group_manager.hpp"
#include "messaging/message_service.hpp"
#include "database/repositories/conversation_repository.hpp"

namespace chat {

GroupManager::GroupManager(GroupService& group_service, MessageService& message_service,
                           ConversationRepository& conversations)
    : group_service_(group_service), message_service_(message_service),
      conversations_(conversations) {}

Result<Group> GroupManager::create_group_with_conversation(const UserID& owner_id,
                                                            const std::string& name) {
    auto group_res = group_service_.create_group(owner_id, name);
    if (!group_res.ok) return group_res;

    auto conv_res = conversations_.create({owner_id});
    if (!conv_res.ok) return Result<Group>::fail(conv_res.error, conv_res.message);

    return group_res;
}

Result<void> GroupManager::add_member_with_access(const GroupID& group_id,
                                                   const UserID& requester_id,
                                                   const UserID& target_user_id) {
    return group_service_.add_member(group_id, requester_id, target_user_id);
}

Result<void> GroupManager::remove_member_with_access(const GroupID& group_id,
                                                      const UserID& requester_id,
                                                      const UserID& target_user_id) {
    return group_service_.remove_member(group_id, requester_id, target_user_id);
}

} // namespace chat
