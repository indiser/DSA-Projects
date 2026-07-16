#include "search/search_service.hpp"

namespace chat {

SearchService::SearchService(UserRepository& users, MessageRepository& messages)
    : users_(users), messages_(messages) {}

Result<std::vector<User>> SearchService::search_users(const std::string& query) {
    return users_.search(query, 20);
}

Result<std::vector<Message>> SearchService::search_messages(const UserID& user_id,
                                                             const std::string& query) {
    // Full-text search via PostgreSQL tsvector — delegated to repository
    // MessageRepository.get_conversation is page-based; a dedicated FTS method
    // would be added to MessageRepository in a future iteration.
    // For now return empty — prevents linker errors while keeping the interface intact.
    (void)user_id;
    (void)query;
    return Result<std::vector<Message>>::success({});
}

} // namespace chat
