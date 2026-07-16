#include "database/repositories/user_repository.hpp"

namespace chat {

UserRepository::UserRepository(DBPool& pool) : pool_(pool) {}

Result<User> UserRepository::create(const std::string& username,
                                     const std::string& email,
                                     const std::string& password_hash) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1(
            "INSERT INTO users (username, email, password_hash) "
            "VALUES ($1, $2, $3) RETURNING id, username, email, password_hash, is_online",
            username, email, password_hash);
        txn.commit();
        User u;
        u.id            = row["id"].as<std::string>();
        u.username      = row["username"].as<std::string>();
        u.email         = row["email"].as<std::string>();
        u.password_hash = row["password_hash"].as<std::string>();
        u.is_online     = row["is_online"].as<bool>();
        return Result<User>::success(u);
    } catch (const std::exception& e) {
        return Result<User>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<User> UserRepository::find_by_id(const UserID& id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1(
            "SELECT id, username, email, password_hash, is_online FROM users WHERE id = $1", id);
        User u;
        u.id            = row["id"].as<std::string>();
        u.username      = row["username"].as<std::string>();
        u.email         = row["email"].as<std::string>();
        u.password_hash = row["password_hash"].as<std::string>();
        u.is_online     = row["is_online"].as<bool>();
        return Result<User>::success(u);
    } catch (const pqxx::unexpected_rows&) {
        return Result<User>::fail(ErrorCode::NOT_FOUND, "user not found");
    } catch (const std::exception& e) {
        return Result<User>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<User> UserRepository::find_by_email(const std::string& email) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1(
            "SELECT id, username, email, password_hash, is_online FROM users WHERE email = $1", email);
        User u;
        u.id            = row["id"].as<std::string>();
        u.username      = row["username"].as<std::string>();
        u.email         = row["email"].as<std::string>();
        u.password_hash = row["password_hash"].as<std::string>();
        u.is_online     = row["is_online"].as<bool>();
        return Result<User>::success(u);
    } catch (const pqxx::unexpected_rows&) {
        return Result<User>::fail(ErrorCode::NOT_FOUND, "user not found");
    } catch (const std::exception& e) {
        return Result<User>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<void> UserRepository::set_online(const UserID& id, bool online) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec_params("UPDATE users SET is_online = $1, updated_at = NOW() WHERE id = $2",
                        online, id);
        txn.commit();
        return Result<void>::success();
    } catch (const std::exception& e) {
        return Result<void>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<void> UserRepository::update_last_seen(const UserID& id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec_params("UPDATE users SET last_seen = NOW(), updated_at = NOW() WHERE id = $1", id);
        txn.commit();
        return Result<void>::success();
    } catch (const std::exception& e) {
        return Result<void>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<std::vector<User>> UserRepository::search(const std::string& query, int limit) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto rows = txn.exec_params(
            "SELECT id, username, email, password_hash, is_online FROM users "
            "WHERE username ILIKE $1 OR email ILIKE $1 LIMIT $2",
            "%" + query + "%", limit);
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
