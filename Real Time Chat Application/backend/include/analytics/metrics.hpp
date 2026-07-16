#pragma once

#include <string>
#include <atomic>

namespace chat {

class Metrics {
public:
    void increment_messages_sent();
    void increment_active_connections();
    void decrement_active_connections();
    void record_cache_hit();
    void record_cache_miss();

    std::string serialize_prometheus() const;

private:
    std::atomic<int64_t> messages_sent_      {0};
    std::atomic<int64_t> active_connections_ {0};
    std::atomic<int64_t> cache_hits_         {0};
    std::atomic<int64_t> cache_misses_       {0};
};

} // namespace chat
