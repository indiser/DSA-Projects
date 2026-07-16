#pragma once

#include "common/types.hpp"
#include "database/db_pool.hpp"
#include <vector>
#include <optional>

namespace chat {

class ConversationRepository {
public:
    explicit ConversationRepository(DBPool& pool);

    Result<Conversation>              create(const std::vector<UserID>& participants);
    Result<Conversation>              find_by_id(const ConversationID& id);
    Result<std::vector<Conversation>> find_for_user(const UserID& user_id);
    Result<std::optional<Conversation>> find_dm(const UserID& user_a, const UserID& user_b);

private:
    DBPool& pool_;
};

} // namespace chat
