#pragma once

#include <string>

namespace chat {

class LogFormatter {
public:
    static std::string format(const std::string& level, const std::string& component,
                              const std::string& message);
};

} // namespace chat
