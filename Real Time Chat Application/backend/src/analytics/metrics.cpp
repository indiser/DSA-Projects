#include "analytics/metrics.hpp"
#include <sstream>

namespace chat {

void Metrics::increment_messages_sent()       { ++messages_sent_; }
void Metrics::increment_active_connections()  { ++active_connections_; }
void Metrics::decrement_active_connections()  { --active_connections_; }
void Metrics::record_cache_hit()              { ++cache_hits_; }
void Metrics::record_cache_miss()             { ++cache_misses_; }

std::string Metrics::serialize_prometheus() const {
    std::ostringstream ss;
    ss << "# HELP chat_messages_sent_total Total messages sent\n"
       << "# TYPE chat_messages_sent_total counter\n"
       << "chat_messages_sent_total " << messages_sent_.load() << "\n"
       << "# HELP chat_active_connections Current active WebSocket connections\n"
       << "# TYPE chat_active_connections gauge\n"
       << "chat_active_connections " << active_connections_.load() << "\n"
       << "# HELP chat_cache_hits_total Total cache hits\n"
       << "# TYPE chat_cache_hits_total counter\n"
       << "chat_cache_hits_total " << cache_hits_.load() << "\n"
       << "# HELP chat_cache_misses_total Total cache misses\n"
       << "# TYPE chat_cache_misses_total counter\n"
       << "chat_cache_misses_total " << cache_misses_.load() << "\n";
    return ss.str();
}

} // namespace chat
