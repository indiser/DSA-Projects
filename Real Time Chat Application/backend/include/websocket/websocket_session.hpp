#pragma once

#include "../common/types.hpp"
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>

namespace chat {

class AuthService;
class MessageService;
class PresenceService;
class DeliveryManager;
class TypingTracker;
class ConnectionManager;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    WebSocketSession(boost::asio::ip::tcp::socket socket,
                     AuthService& auth, MessageService& messages,
                     PresenceService& presence, DeliveryManager& delivery,
                     TypingTracker& typing, ConnectionManager& connections);

    void run();
    void send(const std::string& payload);

private:
    void do_accept();
    void do_auth(const std::string& raw);
    void do_read();
    void on_message(const std::string& raw);
    void do_write(const std::string& payload);
    void do_ping();
    void close();

    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    AuthService&       auth_;
    MessageService&    messages_;
    PresenceService&   presence_;
    DeliveryManager&   delivery_;
    TypingTracker&     typing_;
    ConnectionManager& connections_;
    UserID             user_id_;
    bool               authenticated_ = false;
};

} // namespace chat
