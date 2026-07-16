#include "logging/log_manager.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace chat {

void LogManager::init(const std::string& log_file, const std::string& level) {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file    = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_file, 50 * 1024 * 1024, 5);
    auto logger  = std::make_shared<spdlog::logger>("chat",
        spdlog::sinks_init_list{console, file});

    if      (level == "trace")    logger->set_level(spdlog::level::trace);
    else if (level == "debug")    logger->set_level(spdlog::level::debug);
    else if (level == "info")     logger->set_level(spdlog::level::info);
    else if (level == "warn")     logger->set_level(spdlog::level::warn);
    else if (level == "error")    logger->set_level(spdlog::level::err);
    else if (level == "critical") logger->set_level(spdlog::level::critical);

    logger->flush_on(spdlog::level::err);
    spdlog::set_default_logger(logger);
    // spdlog::register_logger(logger);
}

void LogManager::flush_all() {
    spdlog::apply_all([](std::shared_ptr<spdlog::logger> l) { l->flush(); });
}

void LogManager::shutdown() {
    spdlog::shutdown();
}

} // namespace chat
