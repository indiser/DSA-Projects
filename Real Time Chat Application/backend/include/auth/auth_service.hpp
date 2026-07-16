#pragma once

#include "../common/types.hpp"
#include "password_hasher.hpp"
#include "jwt_manager.hpp"
#include "session_manager.hpp"
#include "../database/repositories/user_repository.hpp"

namespace chat {

class AuthService {
public:
    AuthService(UserRepository& users, PasswordHasher& hasher,
                JwtManager& jwt, SessionManager& sessions);

    Result<User>         register_user(const std::string& username, const std::string& email, const std::string& password);
    Result<SessionToken> login(const std::string& email, const std::string& password);
    Result<void>         logout(const SessionToken& token);
    Result<User>         authenticate(const SessionToken& token);

private:
    UserRepository& users_;
    PasswordHasher& hasher_;
    JwtManager&     jwt_;
    SessionManager& sessions_;
};

} // namespace chat
