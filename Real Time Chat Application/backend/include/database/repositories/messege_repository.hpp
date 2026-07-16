#pragma once

#include "common/types.hpp"
#include "database/db_pool.hpp"
#include <vector>

namespace chat {

class MessageRepository {
public:
    explicit MessageRepository(DBPool& pool);

    Result<Message>              save(const Message& message);
    Result<std::vector<Message>> get_conversation(const ConversationID& conv_id, int limit, int offset);
    Result<void>                 mark_delivered(const MessageID& id);
    Result<void>                 mark_read(const MessageID& id);

private:
    DBPool& pool_;
};

} // namespace chat
