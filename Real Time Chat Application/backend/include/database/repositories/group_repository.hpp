#pragma once

#include "common/types.hpp"
#include "database/db_pool.hpp"
#include <vector>

namespace chat {

class GroupRepository {
public:
    explicit GroupRepository(DBPool& pool);

    Result<Group>              create(const std::string& name, const UserID& owner_id);
    Result<void>               add_member(const GroupID& group_id, const UserID& user_id);
    Result<void>               remove_member(const GroupID& group_id, const UserID& user_id);
    Result<Group>              find_by_id(const GroupID& id);
    Result<std::vector<User>>  get_members(const GroupID& group_id);

private:
    DBPool& pool_;
};

} // namespace chat
