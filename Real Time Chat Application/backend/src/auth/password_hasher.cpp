#include "auth/password_hasher.hpp"
#include <sodium.h>
#include <stdexcept>

namespace chat {

PasswordHasher::PasswordHasher() {
    if (sodium_init() < 0)
        throw std::runtime_error("libsodium init failed");
}

std::string PasswordHasher::hash(const std::string& password) {
    char hash[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(hash, password.c_str(), password.size(),
                          crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
        throw std::runtime_error("password hashing failed (out of memory)");
    return std::string(hash);
}

bool PasswordHasher::verify(const std::string& password, const std::string& hash) {
    return crypto_pwhash_str_verify(hash.c_str(), password.c_str(), password.size()) == 0;
}

} // namespace chat
