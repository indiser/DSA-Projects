#include "database/repositories/group_repository.hpp"

namespace chat {

GroupRepository::GroupRepository(DBPool& pool) : pool_(pool) {}

Result<Group> GroupRepository::create(const std::string& name, const UserID& owner_id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1(
            "INSERT INTO groups (name, owner_id) VALUES ($1, $2) RETURNING id, name, owner_id",
            name, owner_id);
        txn.commit();
        Group g;
        g.id       = row["id"].as<std::string>();
        g.name     = row["name"].as<std::string>();
        g.owner_id = row["owner_id"].as<std::string>();
        return Result<Group>::success(g);
    } catch (const std::exception& e) {
        return Result<Group>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<void> GroupRepository::add_member(const GroupID& group_id, const UserID& user_id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec_params(
            "INSERT INTO group_members (group_id, user_id) VALUES ($1, $2) ON CONFLICT DO NOTHING",
            group_id, user_id);
        txn.commit();
        return Result<void>::success();
    } catch (const std::exception& e) {
        return Result<void>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<void> GroupRepository::remove_member(const GroupID& group_id, const UserID& user_id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec_params("DELETE FROM group_members WHERE group_id = $1 AND user_id = $2",
                        group_id, user_id);
        txn.commit();
        return Result<void>::success();
    } catch (const std::exception& e) {
        return Result<void>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<Group> GroupRepository::find_by_id(const GroupID& id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1(
            "SELECT id, name, owner_id FROM groups WHERE id = $1", id);
        Group g;
        g.id       = row["id"].as<std::string>();
        g.name     = row["name"].as<std::string>();
        g.owner_id = row["owner_id"].as<std::string>();
        auto members = txn.exec_params(
            "SELECT user_id FROM group_members WHERE group_id = $1", id);
        for (const auto& m : members) g.members.push_back(m["user_id"].as<std::string>());
        return Result<Group>::success(g);
    } catch (const pqxx::unexpected_rows&) {
        return Result<Group>::fail(ErrorCode::NOT_FOUND, "group not found");
    } catch (const std::exception& e) {
        return Result<Group>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<std::vector<User>> GroupRepository::get_members(const GroupID& group_id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto rows = txn.exec_params(
            "SELECT u.id, u.username, u.email, u.password_hash, u.is_online "
            "FROM users u JOIN group_members gm ON gm.user_id = u.id "
            "WHERE gm.group_id = $1", group_id);
        std::vector<User> users;
        for (const auto& row : rows) {
            User u;
            u.id            = row["id"].as<std::string>();
            u.username      = row["username"].as<std::string>();
            u.email         = row["email"].as<std::string>();
            u.password_hash = row["password_hash"].as<std::string>();
            u.is_online     = row["is_online"].as<bool>();
            users.push_back(std::move(u));
        }
        return Result<std::vector<User>>::success(std::move(users));
    } catch (const std::exception& e) {
        return Result<std::vector<User>>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

} // namespace chat
