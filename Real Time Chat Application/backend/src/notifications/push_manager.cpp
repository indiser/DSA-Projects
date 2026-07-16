#include "notifications/push_manager.hpp"
#include "logging/logger.hpp"

namespace chat {

PushManager::PushManager(const std::string& fcm_key, const std::string& apns_cert)
    : fcm_key_(fcm_key), apns_cert_(apns_cert) {}

void PushManager::send(const std::string& device_token, const std::string& title,
                        const std::string& body) {
    if (fcm_key_.empty()) {
        Logger::instance().warn("PushManager: FCM key not configured, skipping push");
        return;
    }
    // TODO: implement HTTP POST to FCM/APNs
    Logger::instance().info("PushManager: push to " + device_token + " — " + title);
}

void PushManager::send_batch(const std::vector<std::string>& device_tokens,
                              const std::string& title, const std::string& body) {
    for (const auto& token : device_tokens)
        send(token, title, body);
}

} // namespace chat
