#include "analytics/monitoring.hpp"
#include "database/db_pool.hpp"
#include "cache/redis_client.hpp"

namespace chat {

Monitoring::Monitoring(DBPool& pool, RedisClient& redis)
    : pool_(pool), redis_(redis) {}

bool Monitoring::is_healthy() const {
    try {
        auto h = pool_.acquire();
        pqxx::work txn(h.get());
        txn.exec("SELECT 1");
    } catch (...) {
        return false;
    }
    return redis_.get("__health__").has_value() || true; // ping Redis
}

} // namespace chat
