#pragma once

#include <string>

namespace chat {

class PasswordHasher {
public:
    PasswordHasher();

    std::string hash(const std::string& password);
    bool        verify(const std::string& password, const std::string& hash);
};

} // namespace chat
