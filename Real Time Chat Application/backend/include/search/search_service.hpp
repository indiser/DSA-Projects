#pragma once

#include "../common/types.hpp"
#include "../database/repositories/user_repository.hpp"
#include "../database/repositories/messege_repository.hpp"
#include <vector>

namespace chat {

class SearchService {
public:
    SearchService(UserRepository& users, MessageRepository& messages);

    Result<std::vector<User>>    search_users(const std::string& query);
    Result<std::vector<Message>> search_messages(const UserID& user_id, const std::string& query);

private:
    UserRepository&    users_;
    MessageRepository& messages_;
};

} // namespace chat
