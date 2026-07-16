#include "logging/log_manager.hpp"
#include "logging/logger.hpp"
#include "database/db_pool.hpp"
#include "cache/redis_client.hpp"
#include "cache/cache_manager.hpp"
#include "database/repositories/user_repository.hpp"
#include "database/repositories/messege_repository.hpp"
#include "database/repositories/conversation_repository.hpp"
#include "database/repositories/group_repository.hpp"
#include "auth/password_hasher.hpp"
#include "auth/jwt_manager.hpp"
#include "auth/session_manager.hpp"
#include "auth/auth_service.hpp"
#include "presence/presence_service.hpp"
#include "presence/online_tracker.hpp"
#include "presence/typing_tracker.hpp"
#include "notifications/push_manager.hpp"
#include "notifications/notification_service.hpp"
#include "messaging/message_ordering.hpp"
#include "messaging/message_queue.hpp"
#include "messaging/delivery_manager.hpp"
#include "messaging/message_dispatcher.hpp"
#include "messaging/message_service.hpp"
#include "groups/group_service.hpp"
#include "groups/group_manager.hpp"
#include "websocket/connection_manager.hpp"
#include "websocket/websocket_server.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <cstdlib>

static std::string env(const char* key, const std::string& fallback = "") {
    const char* v = std::getenv(key);
    return v ? v : fallback;
}

int main(int argc, char* argv[]) {
    std::string config_path = "config/dev.yaml";
    if (argc > 2 && std::string(argv[1]) == "--config") config_path = argv[2];

    // 1. Load config
    YAML::Node cfg = YAML::LoadFile(config_path);

    // 2. Init logger
    chat::LogManager::init(
        cfg["logging"]["file"].as<std::string>("logs/app.log"),
        cfg["logging"]["level"].as<std::string>("info"));
    chat::Logger::instance().info("Starting chat server");

    // 3. DB pool
    chat::DBConfig db_cfg;
    db_cfg.host      = env("DB_HOST", cfg["database"]["host"].as<std::string>("127.0.0.1"));
    db_cfg.port      = std::stoi(env("DB_PORT", std::to_string(cfg["database"]["port"].as<int>(5432))));
    db_cfg.user      = env("DB_USER", cfg["database"]["user"].as<std::string>("chat_user"));
    db_cfg.password  = env("DB_PASS", cfg["database"]["password"].as<std::string>(""));
    db_cfg.dbname    = env("DB_NAME", cfg["database"]["dbname"].as<std::string>("chat_db"));
    db_cfg.pool_size = cfg["database"]["pool_size"].as<int>(10);
    chat::DBPool db_pool(db_cfg);

    // 4. Redis client
    chat::RedisConfig redis_cfg;
    redis_cfg.host     = env("REDIS_HOST", cfg["redis"]["host"].as<std::string>("127.0.0.1"));
    redis_cfg.port     = std::stoi(env("REDIS_PORT", std::to_string(cfg["redis"]["port"].as<int>(6379))));
    redis_cfg.password = env("REDIS_PASS", cfg["redis"]["password"].as<std::string>(""));
    redis_cfg.db       = cfg["redis"]["db"].as<int>(0);
    chat::RedisClient redis(redis_cfg);

    // 5. Cache manager
    chat::CacheManager cache(redis);

    // 6. Repositories
    chat::UserRepository         user_repo(db_pool);
    chat::MessageRepository      msg_repo(db_pool);
    chat::ConversationRepository conv_repo(db_pool);
    chat::GroupRepository        group_repo(db_pool);

    // 7. Auth
    chat::PasswordHasher hasher;
    chat::JwtManager     jwt(
        env("JWT_SECRET", cfg["jwt"]["secret"].as<std::string>("dev_secret")),
        cfg["jwt"]["expiry_seconds"].as<int>(86400));
    chat::SessionManager sessions(redis);
    chat::AuthService    auth(user_repo, hasher, jwt, sessions);

    // 8. Presence
    chat::PresenceService presence(redis);
    chat::OnlineTracker   online_tracker(presence);

    // 9. Notifications
    chat::PushManager        push(
        cfg["push"]["fcm_key"].as<std::string>(""),
        cfg["push"]["apns_cert"].as<std::string>(""));
    chat::NotificationService notifications(push);

    // 10. Messaging
    chat::ConnectionManager  connections;
    chat::TypingTracker      typing(redis, connections);
    chat::MessageOrdering    ordering(redis);
    chat::MessageQueue       msg_queue;
    chat::DeliveryManager    delivery(msg_repo);
    chat::MessageDispatcher  dispatcher(connections, presence, notifications);
    chat::MessageService     messages(msg_repo, conv_repo, cache, dispatcher);

    // 11. Groups
    chat::GroupService  group_service(group_repo);
    chat::GroupManager  group_manager(group_service, messages, conv_repo);

    // 12. WebSocket server
    chat::ServerConfig srv_cfg;
    srv_cfg.host            = cfg["server"]["host"].as<std::string>("127.0.0.1");
    srv_cfg.port            = cfg["server"]["port"].as<int>(8080);
    srv_cfg.threads         = cfg["server"]["threads"].as<int>(4);
    srv_cfg.max_connections = cfg["server"]["max_connections"].as<int>(1000);

    chat::WebSocketServer server(srv_cfg, auth, messages, presence,
                                  delivery, typing, connections);

    chat::Logger::instance().info("Listening on " + srv_cfg.host +
                                   ":" + std::to_string(srv_cfg.port));
    server.run(); // blocks

    chat::LogManager::shutdown();
    return 0;
}
