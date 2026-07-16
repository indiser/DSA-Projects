#include "auth/session_manager.hpp"
#include "common/utils.hpp"
#include "common/constants.hpp"

namespace chat {

SessionManager::SessionManager(RedisClient& redis) : redis_(redis) {}

SessionToken SessionManager::create_session(const UserID& user_id) {
    SessionToken token = utils::generate_uuid();
    redis_.set("session:" + token, user_id, constants::SESSION_TTL_SECONDS);
    return token;
}

Result<UserID> SessionManager::get_user(const SessionToken& token) {
    auto val = redis_.get("session:" + token);
    if (!val) return Result<UserID>::fail(ErrorCode::UNAUTHORIZED, "session not found");
    return Result<UserID>::success(*val);
}

Result<void> SessionManager::invalidate(const SessionToken& token) {
    redis_.del("session:" + token);
    return Result<void>::success();
}

Result<void> SessionManager::refresh(const SessionToken& token) {
    bool ok = redis_.expire("session:" + token, constants::SESSION_TTL_SECONDS);
    if (!ok) return Result<void>::fail(ErrorCode::NOT_FOUND, "session not found");
    return Result<void>::success();
}

} // namespace chat
