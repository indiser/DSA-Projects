#include "websocket/websocket_server.hpp"
#include "websocket/websocket_session.hpp"
#include "auth/auth_service.hpp"
#include "messaging/message_service.hpp"
#include "messaging/delivery_manager.hpp"
#include "presence/presence_service.hpp"
#include "presence/typing_tracker.hpp"
#include "websocket/connection_manager.hpp"
#include <boost/asio/strand.hpp>
#include <thread>
#include <vector>

namespace chat {

namespace net = boost::asio;

WebSocketServer::WebSocketServer(const ServerConfig& config,
                                 AuthService& auth, MessageService& messages,
                                 PresenceService& presence, DeliveryManager& delivery,
                                 TypingTracker& typing, ConnectionManager& connections)
    : config_(config),
      acceptor_(ioc_, {net::ip::make_address(config.host),
                       static_cast<unsigned short>(config.port)}),
      auth_(auth), messages_(messages), presence_(presence),
      delivery_(delivery), typing_(typing), connections_(connections) {}

void WebSocketServer::run() {
    do_accept();
    std::vector<std::thread> threads;
    threads.reserve(config_.threads - 1);
    for (int i = 0; i < config_.threads - 1; ++i)
        threads.emplace_back([this] { ioc_.run(); });
    ioc_.run();
    for (auto& t : threads) t.join();
}

void WebSocketServer::stop() {
    ioc_.stop();
}

void WebSocketServer::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec,
                                   net::ip::tcp::socket socket) {
        if (!ec) {
            std::make_shared<WebSocketSession>(
                std::move(socket), auth_, messages_, presence_,
                delivery_, typing_, connections_)->run();
        }
        do_accept();
    });
}

} // namespace chat
