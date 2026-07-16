#include "database/repositories/messege_repository.hpp"

namespace chat {

MessageRepository::MessageRepository(DBPool& pool) : pool_(pool) {}

Result<Message> MessageRepository::save(const Message& msg) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto row = txn.exec_params1(
            "INSERT INTO messages (conversation_id, sender_id, content, sequence_id) "
            "VALUES ($1, $2, $3, $4) RETURNING id, conversation_id, sender_id, content, delivered, read, "
            "EXTRACT(EPOCH FROM created_at)::bigint AS timestamp",
            msg.conversation_id, msg.sender_id, msg.content, msg.timestamp);
        txn.commit();
        Message m;
        m.id              = row["id"].as<std::string>();
        m.conversation_id = row["conversation_id"].as<std::string>();
        m.sender_id       = row["sender_id"].as<std::string>();
        m.content         = row["content"].as<std::string>();
        m.delivered       = row["delivered"].as<bool>();
        m.read            = row["read"].as<bool>();
        m.timestamp       = row["timestamp"].as<int64_t>();
        return Result<Message>::success(m);
    } catch (const std::exception& e) {
        return Result<Message>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<std::vector<Message>> MessageRepository::get_conversation(
    const ConversationID& conv_id, int limit, int offset) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        auto rows = txn.exec_params(
            "SELECT id, conversation_id, sender_id, content, delivered, read, "
            "EXTRACT(EPOCH FROM created_at)::bigint AS timestamp "
            "FROM messages WHERE conversation_id = $1 "
            "ORDER BY created_at DESC LIMIT $2 OFFSET $3",
            conv_id, limit, offset);
        std::vector<Message> msgs;
        for (const auto& row : rows) {
            Message m;
            m.id              = row["id"].as<std::string>();
            m.conversation_id = row["conversation_id"].as<std::string>();
            m.sender_id       = row["sender_id"].as<std::string>();
            m.content         = row["content"].as<std::string>();
            m.delivered       = row["delivered"].as<bool>();
            m.read            = row["read"].as<bool>();
            m.timestamp       = row["timestamp"].as<int64_t>();
            msgs.push_back(std::move(m));
        }
        return Result<std::vector<Message>>::success(std::move(msgs));
    } catch (const std::exception& e) {
        return Result<std::vector<Message>>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<void> MessageRepository::mark_delivered(const MessageID& id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec_params("UPDATE messages SET delivered = TRUE WHERE id = $1", id);
        txn.commit();
        return Result<void>::success();
    } catch (const std::exception& e) {
        return Result<void>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

Result<void> MessageRepository::mark_read(const MessageID& id) {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec_params("UPDATE messages SET read = TRUE WHERE id = $1", id);
        txn.commit();
        return Result<void>::success();
    } catch (const std::exception& e) {
        return Result<void>::fail(ErrorCode::DB_ERROR, e.what());
    }
}

} // namespace chat
