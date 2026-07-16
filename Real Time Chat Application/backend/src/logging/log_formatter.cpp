#include "logging/log_formatter.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace chat {

std::string LogFormatter::format(const std::string& level, const std::string& component,
                                  const std::string& message) {
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ")
       << " [" << level << "] [" << component << "] " << message;
    return ss.str();
}

} // namespace chat
