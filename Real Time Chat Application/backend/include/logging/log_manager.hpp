#pragma once

#include <string>

namespace chat {

class LogManager {
public:
    static void init(const std::string& log_file, const std::string& level);
    static void flush_all();
    static void shutdown();
};

} // namespace chat
