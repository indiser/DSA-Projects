#include "auth/auth_service.hpp"
#include "common/utils.hpp"

namespace chat {

AuthService::AuthService(UserRepository& users, PasswordHasher& hasher,
                         JwtManager& jwt, SessionManager& sessions)
    : users_(users), hasher_(hasher), jwt_(jwt), sessions_(sessions) {}

Result<User> AuthService::register_user(const std::string& username,
                                         const std::string& email,
                                         const std::string& password) {
    if (!utils::is_valid_username(username))
        return Result<User>::fail(ErrorCode::INVALID_INPUT, "invalid username");
    if (!utils::is_valid_email(email))
        return Result<User>::fail(ErrorCode::INVALID_INPUT, "invalid email");
    if (password.size() < 8)
        return Result<User>::fail(ErrorCode::INVALID_INPUT, "password too short");

    std::string hash = hasher_.hash(password);
    return users_.create(username, email, hash);
}

Result<SessionToken> AuthService::login(const std::string& email, const std::string& password) {
    auto res = users_.find_by_email(email);
    if (!res.ok) return Result<SessionToken>::fail(ErrorCode::UNAUTHORIZED, "invalid credentials");

    if (!hasher_.verify(password, res.value.password_hash))
        return Result<SessionToken>::fail(ErrorCode::UNAUTHORIZED, "invalid credentials");

    SessionToken token = sessions_.create_session(res.value.id);
    return Result<SessionToken>::success(token);
}

Result<void> AuthService::logout(const SessionToken& token) {
    return sessions_.invalidate(token);
}

Result<User> AuthService::authenticate(const SessionToken& token) {
    auto uid = sessions_.get_user(token);
    if (!uid.ok) return Result<User>::fail(uid.error, uid.message);
    return users_.find_by_id(uid.value);
}

} // namespace chat
