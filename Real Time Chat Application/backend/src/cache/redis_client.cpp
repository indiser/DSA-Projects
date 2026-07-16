#include "cache/redis_client.hpp"
#include <hiredis/hiredis.h>
#include <stdexcept>
#include <thread>

namespace chat {

struct RedisClient::Impl {
    redisContext* ctx = nullptr;

    explicit Impl(const RedisConfig& cfg) {
        ctx = redisConnect(cfg.host.c_str(), cfg.port);
        if (!ctx || ctx->err)
            throw std::runtime_error(ctx ? ctx->errstr : "redis connect failed");
        if (!cfg.password.empty()) {
            auto* r = static_cast<redisReply*>(redisCommand(ctx, "AUTH %s", cfg.password.c_str()));
            freeReplyObject(r);
        }
        if (cfg.db != 0) {
            auto* r = static_cast<redisReply*>(redisCommand(ctx, "SELECT %d", cfg.db));
            freeReplyObject(r);
        }
    }

    ~Impl() { if (ctx) redisFree(ctx); }
};

RedisClient::RedisClient(const RedisConfig& config) : impl_(std::make_unique<Impl>(config)) {}
RedisClient::~RedisClient() = default;

std::optional<std::string> RedisClient::get(const std::string& key) {
    auto* r = static_cast<redisReply*>(redisCommand(impl_->ctx, "GET %s", key.c_str()));
    if (!r) return std::nullopt;
    std::optional<std::string> val;
    if (r->type == REDIS_REPLY_STRING) val = std::string(r->str, r->len);
    freeReplyObject(r);
    return val;
}

bool RedisClient::set(const std::string& key, const std::string& value, int ttl_seconds) {
    redisReply* r;
    if (ttl_seconds > 0)
        r = static_cast<redisReply*>(redisCommand(impl_->ctx, "SET %s %b EX %d",
            key.c_str(), value.data(), value.size(), ttl_seconds));
    else
        r = static_cast<redisReply*>(redisCommand(impl_->ctx, "SET %s %b",
            key.c_str(), value.data(), value.size()));
    bool ok = r && r->type != REDIS_REPLY_ERROR;
    if (r) freeReplyObject(r);
    return ok;
}

bool RedisClient::del(const std::string& key) {
    auto* r = static_cast<redisReply*>(redisCommand(impl_->ctx, "DEL %s", key.c_str()));
    bool ok = r && r->type == REDIS_REPLY_INTEGER;
    if (r) freeReplyObject(r);
    return ok;
}

bool RedisClient::expire(const std::string& key, int ttl_seconds) {
    auto* r = static_cast<redisReply*>(redisCommand(impl_->ctx, "EXPIRE %s %d",
        key.c_str(), ttl_seconds));
    bool ok = r && r->integer == 1;
    if (r) freeReplyObject(r);
    return ok;
}

bool RedisClient::publish(const std::string& channel, const std::string& message) {
    auto* r = static_cast<redisReply*>(redisCommand(impl_->ctx, "PUBLISH %s %s",
        channel.c_str(), message.c_str()));
    bool ok = r && r->type == REDIS_REPLY_INTEGER;
    if (r) freeReplyObject(r);
    return ok;
}

void RedisClient::subscribe(const std::string& channel,
                             std::function<void(const std::string&)> handler) {
    redisCommand(impl_->ctx, "SUBSCRIBE %s", channel.c_str());
    std::thread([this, handler]() {
        redisReply* r = nullptr;
        while (redisGetReply(impl_->ctx, reinterpret_cast<void**>(&r)) == REDIS_OK) {
            if (r && r->type == REDIS_REPLY_ARRAY && r->elements == 3)
                handler(std::string(r->element[2]->str, r->element[2]->len));
            if (r) freeReplyObject(r);
        }
    }).detach();
}

} // namespace chat
