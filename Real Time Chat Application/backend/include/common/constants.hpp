#pragma once

namespace chat::constants {

constexpr int    MAX_MESSAGE_LENGTH      = 4096;
constexpr int    MAX_GROUP_SIZE          = 500;
constexpr int    JWT_EXPIRY_SECONDS      = 86400;
constexpr int    SESSION_TTL_SECONDS     = 86400;
constexpr int    PRESENCE_TTL_SECONDS    = 30;
constexpr int    PRESENCE_REFRESH_SECS   = 20;
constexpr int    TYPING_TTL_SECONDS      = 5;
constexpr int    WS_PING_INTERVAL_SECS   = 20;
constexpr int    WS_PONG_TIMEOUT_SECS    = 10;
constexpr int    MESSAGE_HISTORY_LIMIT   = 50;
constexpr int    DB_POOL_SIZE            = 10;
constexpr int    REDIS_POOL_SIZE         = 10;
constexpr int    SERVER_PORT             = 8080;

} // namespace chat::constants
