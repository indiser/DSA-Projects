#pragma once

namespace chat {

class DBPool;
class RedisClient;

class Monitoring {
public:
    Monitoring(DBPool& pool, RedisClient& redis);

    bool is_healthy() const;

private:
    DBPool&      pool_;
    RedisClient& redis_;
};

} // namespace chat
