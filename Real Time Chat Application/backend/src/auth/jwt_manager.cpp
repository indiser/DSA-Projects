#include "auth/jwt_manager.hpp"
#include <jwt-cpp/jwt.h>

namespace chat {

JwtManager::JwtManager(const std::string& secret, int expiry_seconds)
    : secret_(secret), expiry_seconds_(expiry_seconds) {}

std::string JwtManager::generate(const UserID& user_id) {
    return jwt::create()
        .set_issuer("chat")
        .set_subject(user_id)
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() +
                        std::chrono::seconds(expiry_seconds_))
        .sign(jwt::algorithm::hs256{secret_});
}

Result<UserID> JwtManager::verify(const std::string& token) {
    try {
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret_})
            .with_issuer("chat");
        auto decoded = jwt::decode(token);
        verifier.verify(decoded);
        return Result<UserID>::success(decoded.get_subject());
    } catch (const std::exception& e) {
        return Result<UserID>::fail(ErrorCode::UNAUTHORIZED, e.what());
    }
}

} // namespace chat
