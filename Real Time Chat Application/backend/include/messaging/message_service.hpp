#pragma once

#include "../common/types.hpp"
#include "../database/repositories/messege_repository.hpp"
#include "../database/repositories/conversation_repository.hpp"
#include "../cache/cache_manager.hpp"
#include <vector>

namespace chat {

class MessageDispatcher;

class MessageService {
public:
    MessageService(MessageRepository& messages, ConversationRepository& conversations,
                   CacheManager& cache, MessageDispatcher& dispatcher);

    Result<Message>              send_message(const UserID& sender_id, const ConversationID& conv_id, const std::string& content);
    Result<std::vector<Message>> get_history(const UserID& user_id, const ConversationID& conv_id, int limit, int offset);

private:
    MessageRepository&      messages_;
    ConversationRepository& conversations_;
    CacheManager&           cache_;
    MessageDispatcher&      dispatcher_;
};

} // namespace chat
