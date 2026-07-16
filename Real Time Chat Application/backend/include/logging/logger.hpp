#pragma once

#include <string>
#include <memory>

namespace spdlog { class logger; }

namespace chat {

class Logger {
public:
    static Logger& instance();

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

private:
    Logger();
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace chat
