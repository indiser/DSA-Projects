#pragma once

#include <string>
#include <chrono>

namespace chat::utils {

std::string generate_uuid();
int64_t     now_timestamp();
std::string trim(const std::string& s);
bool        is_valid_email(const std::string& email);
bool        is_valid_username(const std::string& username);
std::string sanitize(const std::string& input);

} // namespace chat::utils
