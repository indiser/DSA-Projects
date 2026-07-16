#pragma once

#include "../common/types.hpp"
#include <string>

namespace chat {

class JwtManager {
public:
    explicit JwtManager(const std::string& secret, int expiry_seconds);

    std::string     generate(const UserID& user_id);
    Result<UserID>  verify(const std::string& token);

private:
    std::string secret_;
    int         expiry_seconds_;
};

} // namespace chat
