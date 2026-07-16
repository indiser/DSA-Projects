#include "logging/logger.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <stdexcept>

namespace chat {

// Logger::Logger() {
//     auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//     auto file    = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
//         "logs/app.log", 50 * 1024 * 1024, 5);
//     logger_ = std::make_shared<spdlog::logger>("chat",
//         spdlog::sinks_init_list{console, file});
//     logger_->set_level(spdlog::level::debug);
//     logger_->flush_on(spdlog::level::err);
// }

Logger::Logger() {
    logger_ = spdlog::get("chat");

    if (!logger_) {
        throw std::runtime_error("Logger not initialized");
    }
}

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::debug(const std::string& msg) { logger_->debug(msg); }
void Logger::info(const std::string& msg)  { logger_->info(msg);  }
void Logger::warn(const std::string& msg)  { logger_->warn(msg);  }
void Logger::error(const std::string& msg) { logger_->error(msg); }

} // namespace chat
