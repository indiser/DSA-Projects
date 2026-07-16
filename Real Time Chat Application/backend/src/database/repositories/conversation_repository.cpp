#include "database/repositories/conversation_repository.hpp"

namespace chat {

ConversationRepository::ConversationRepository(DBPool& pool) : pool_(pool) {}

Result<Conversation> ConversationRepository::create(const std::vector<UserID>& participants) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1("INSERT INTO conversations DEFAULT VALUES RETURNING id");
        std::string conv_id = row["id"].as<std::string>();
        for (const auto& uid : participants)
            txn.exec_params("INSERT INTO conversation_participants (conversation_id, user_id) VALUES ($1, $2)",
                            conv_id, uid);
        txn.commit();
        Conversation c;
        c.id           = conv_id;
        c.participants = participants;
        return Result<Conversation>::success(c);
    } catch (const std::exception& e) {
        return Result<Conversation>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<Conversation> ConversationRepository::find_by_id(const ConversationID& id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto rows = txn.exec_params(
            "SELECT cp.user_id FROM conversation_participants cp WHERE cp.conversation_id = $1", id);
        if (rows.empty()) return Result<Conversation>::fail(ErrorCode::NOT_FOUND, "conversation not found");
        Conversation c;
        c.id = id;
        for (const auto& row : rows) c.participants.push_back(row["user_id"].as<std::string>());
        return Result<Conversation>::success(c);
    } catch (const std::exception& e) {
        return Result<Conversation>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<std::vector<Conversation>> ConversationRepository::find_for_user(const UserID& user_id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto rows = txn.exec_params(
            "SELECT conversation_id FROM conversation_participants WHERE user_id = $1", user_id);
        std::vector<Conversation> convs;
        for (const auto& row : rows) {
            auto res = find_by_id(row["conversation_id"].as<std::string>());
            if (res.ok) convs.push_back(std::move(res.value));
        }
        return Result<std::vector<Conversation>>::success(std::move(convs));
    } catch (const std::exception& e) {
        return Result<std::vector<Conversation>>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<std::optional<Conversation>> ConversationRepository::find_dm(
    const UserID& user_a, const UserID& user_b) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto rows = txn.exec_params(
            "SELECT c.id FROM conversations c "
            "JOIN conversation_participants p1 ON p1.conversation_id = c.id AND p1.user_id = $1 "
            "JOIN conversation_participants p2 ON p2.conversation_id = c.id AND p2.user_id = $2 "
            "WHERE c.group_id IS NULL",
            user_a, user_b);
        if (rows.empty()) return Result<std::optional<Conversation>>::success(std::nullopt);
        auto res = find_by_id(rows[0]["id"].as<std::string>());
        if (!res.ok) return Result<std::optional<Conversation>>::fail(res.error, res.message);
        return Result<std::optional<Conversation>>::success(std::move(res.value));
    } catch (const std::exception& e) {
        return Result<std::optional<Conversation>>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

} // namespace chat
