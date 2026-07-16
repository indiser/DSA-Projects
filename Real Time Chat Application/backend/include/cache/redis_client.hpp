#pragma once

#include <string>
#include <optional>
#include <functional>
#include <memory>

namespace chat {

struct RedisConfig {
    std::string host        = "localhost";
    int         port        = 6379;
    std::string password;
    int         db          = 0;
    int         pool_size   = 10;
};

class RedisClient {
public:
    explicit RedisClient(const RedisConfig& config);
    ~RedisClient();

    std::optional<std::string> get(const std::string& key);
    bool set(const std::string& key, const std::string& value, int ttl_seconds = 0);
    bool del(const std::string& key);
    bool expire(const std::string& key, int ttl_seconds);
    bool publish(const std::string& channel, const std::string& message);
    void subscribe(const std::string& channel, std::function<void(const std::string&)> handler);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace chat
