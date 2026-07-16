#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <memory>

namespace chat {

class AuthService;
class MessageService;
class PresenceService;
class DeliveryManager;
class TypingTracker;
class ConnectionManager;

struct ServerConfig {
    std::string host            = "0.0.0.0";
    int         port            = 8080;
    int         threads         = 4;
    int         max_connections = 1000;
};

class WebSocketServer {
public:
    WebSocketServer(const ServerConfig& config,
                    AuthService& auth, MessageService& messages,
                    PresenceService& presence, DeliveryManager& delivery,
                    TypingTracker& typing, ConnectionManager& connections);

    void run();
    void stop();

private:
    void do_accept();

    ServerConfig                        config_;
    boost::asio::io_context             ioc_;
    boost::asio::ip::tcp::acceptor      acceptor_;
    AuthService&                        auth_;
    MessageService&                     messages_;
    PresenceService&                    presence_;
    DeliveryManager&                    delivery_;
    TypingTracker&                      typing_;
    ConnectionManager&                  connections_;
};

} // namespace chat
