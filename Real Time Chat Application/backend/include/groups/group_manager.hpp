#pragma once

#include "../common/types.hpp"
#include "group_service.hpp"

namespace chat {

class MessageService;
class ConversationRepository;

class GroupManager {
public:
    GroupManager(GroupService& group_service, MessageService& message_service,
                 ConversationRepository& conversations);

    Result<Group>        create_group_with_conversation(const UserID& owner_id, const std::string& name);
    Result<void>         add_member_with_access(const GroupID& group_id, const UserID& requester_id, const UserID& target_user_id);
    Result<void>         remove_member_with_access(const GroupID& group_id, const UserID& requester_id, const UserID& target_user_id);

private:
    GroupService&           group_service_;
    MessageService&         message_service_;
    ConversationRepository& conversations_;
};

} // namespace chat
