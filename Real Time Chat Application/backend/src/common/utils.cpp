#include "common/utils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <regex>

namespace chat::utils {

std::string generate_uuid() {
    boost::uuids::random_generator gen;
    return boost::uuids::to_string(gen());
}

int64_t now_timestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) ++start;
    auto end = s.end();
    while (end != start && std::isspace(*(end - 1))) --end;
    return std::string(start, end);
}

bool is_valid_email(const std::string& email) {
    static const std::regex pattern(R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
}

bool is_valid_username(const std::string& username) {
    if (username.size() < 3 || username.size() > 50) return false;
    return std::all_of(username.begin(), username.end(), [](char c) {
        return std::isalnum(c) || c == '_' || c == '-';
    });
}

std::string sanitize(const std::string& input) {
    std::string out;
    out.reserve(input.size());
    for (char c : input) {
        if (c != '<' && c != '>' && c != '\0') out += c;
    }
    return out;
}

} // namespace chat::utils
