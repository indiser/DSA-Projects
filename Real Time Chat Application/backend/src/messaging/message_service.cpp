#include "messaging/message_service.hpp"
#include "messaging/message_dispatcher.hpp"
#include "common/constants.hpp"
#include "common/utils.hpp"

namespace chat {

MessageService::MessageService(MessageRepository& messages,
                                ConversationRepository& conversations,
                                CacheManager& cache,
                                MessageDispatcher& dispatcher)
    : messages_(messages), conversations_(conversations),
      cache_(cache), dispatcher_(dispatcher) {}

Result<Message> MessageService::send_message(const UserID& sender_id,
                                              const ConversationID& conv_id,
                                              const std::string& content) {
    if (content.empty())
        return Result<Message>::fail(ErrorCode::INVALID_INPUT, "content is empty");
    if (content.size() > constants::MAX_MESSAGE_LENGTH)
        return Result<Message>::fail(ErrorCode::INVALID_INPUT, "content too long");

    // Verify sender is a participant
    auto conv = conversations_.find_by_id(conv_id);
    if (!conv.ok) return Result<Message>::fail(conv.error, conv.message);

    bool is_participant = false;
    for (const auto& uid : conv.value.participants)
        if (uid == sender_id) { is_participant = true; break; }
    if (!is_participant)
        return Result<Message>::fail(ErrorCode::FORBIDDEN, "not a participant");

    Message msg;
    msg.conversation_id = conv_id;
    msg.sender_id       = sender_id;
    msg.content         = content;
    msg.timestamp       = utils::now_timestamp();

    auto saved = messages_.save(msg);
    if (!saved.ok) return saved;

    cache_.invalidate_messages(conv_id);
    dispatcher_.dispatch(saved.value);

    return saved;
}

Result<std::vector<Message>> MessageService::get_history(const UserID& user_id,
                                                          const ConversationID& conv_id,
                                                          int limit, int offset) {
    // Check cache only for first page
    if (offset == 0) {
        auto cached = cache_.get_cached_messages(conv_id);
        if (cached) return Result<std::vector<Message>>::success(std::move(*cached));
    }

    auto result = messages_.get_conversation(conv_id, limit, offset);
    if (!result.ok) return result;

    if (offset == 0) cache_.cache_messages(conv_id, result.value);
    return result;
}

} // namespace chat
