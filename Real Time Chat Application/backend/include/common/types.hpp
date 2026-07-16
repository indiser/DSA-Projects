#pragma once

#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace chat {

using UserID         = std::string;
using MessageID      = std::string;
using ConversationID = std::string;
using GroupID        = std::string;
using SessionToken   = std::string;

struct User {
    UserID      id;
    std::string username;
    std::string email;
    std::string password_hash;
    bool        is_online    = false;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    std::chrono::system_clock::time_point last_seen;
};

struct Message {
    MessageID      id;
    ConversationID conversation_id;
    UserID         sender_id;
    std::string    content;
    bool           delivered  = false;
    bool           read       = false;
    int64_t        timestamp  = 0;
};

struct Conversation {
    ConversationID       id;
    std::vector<UserID>  participants;
    std::optional<GroupID> group_id;
};

struct Group {
    GroupID             id;
    std::string         name;
    UserID              owner_id;
    std::vector<UserID> members;
};

// Result<T> — wraps a value or an error
enum class ErrorCode {
    OK,
    NOT_FOUND,
    UNAUTHORIZED,
    FORBIDDEN,
    INVALID_INPUT,
    DB_ERROR,
    CACHE_ERROR,
    INTERNAL
};

template<typename T>
struct Result {
    bool      ok;
    T         value;
    ErrorCode error = ErrorCode::OK;
    std::string message;

    static Result success(T val) { return {true, std::move(val)}; }
    static Result fail(ErrorCode ec, std::string msg = {}) {
        Result r;
        r.ok      = false;
        r.error   = ec;
        r.message = std::move(msg);
        return r;
    }
};

template<>
struct Result<void> {
    bool        ok;
    ErrorCode   error = ErrorCode::OK;
    std::string message;

    static Result success()                              { return {true}; }
    static Result fail(ErrorCode ec, std::string msg = {}) {
        return {false, ec, std::move(msg)};
    }
};

} // namespace chat
