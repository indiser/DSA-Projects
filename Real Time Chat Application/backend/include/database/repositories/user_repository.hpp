#pragma once

#include "common/types.hpp"
#include "database/db_pool.hpp"
#include <string>
#include <vector>

namespace chat {

class UserRepository {
public:
    explicit UserRepository(DBPool& pool);

    Result<User>              create(const std::string& username, const std::string& email, const std::string& password_hash);
    Result<User>              find_by_id(const UserID& id);
    Result<User>              find_by_email(const std::string& email);
    Result<void>              set_online(const UserID& id, bool online);
    Result<void>              update_last_seen(const UserID& id);
    Result<std::vector<User>> search(const std::string& query, int limit);

private:
    DBPool& pool_;
};

} // namespace chat
