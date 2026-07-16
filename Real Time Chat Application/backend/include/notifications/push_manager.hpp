#pragma once

#include "../common/types.hpp"
#include <string>
#include <vector>

namespace chat {

class PushManager {
public:
    PushManager(const std::string& fcm_key, const std::string& apns_cert);

    void send(const std::string& device_token, const std::string& title, const std::string& body);
    void send_batch(const std::vector<std::string>& device_tokens, const std::string& title, const std::string& body);

private:
    std::string fcm_key_;
    std::string apns_cert_;
};

} // namespace chat
