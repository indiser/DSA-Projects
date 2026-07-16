#include "websocket/websocket_session.hpp"
#include "auth/auth_service.hpp"
#include "messaging/message_service.hpp"
#include "messaging/delivery_manager.hpp"
#include "presence/presence_service.hpp"
#include "presence/typing_tracker.hpp"
#include "websocket/connection_manager.hpp"
#include <boost/beast/core.hpp>
#include <nlohmann/json.hpp>

namespace chat {

namespace beast = boost::beast;
namespace net   = boost::asio;
using json      = nlohmann::json;

WebSocketSession::WebSocketSession(net::ip::tcp::socket socket,
                                   AuthService& auth, MessageService& messages,
                                   PresenceService& presence, DeliveryManager& delivery,
                                   TypingTracker& typing, ConnectionManager& connections)
    : ws_(std::move(socket)), auth_(auth), messages_(messages),
      presence_(presence), delivery_(delivery), typing_(typing), connections_(connections) {}

void WebSocketSession::run() {
    do_accept();
}

void WebSocketSession::do_accept() {
    auto self = shared_from_this();
    ws_.async_accept([self](beast::error_code ec) {
        if (ec) return;
        self->do_read();
    });
}

void WebSocketSession::do_read() {
    auto self = shared_from_this();
    auto buffer = std::make_shared<beast::flat_buffer>();
    ws_.async_read(*buffer, [self, buffer](beast::error_code ec, std::size_t) {
        if (ec) { self->close(); return; }
        std::string raw = beast::buffers_to_string(buffer->data());
        if (!self->authenticated_)
            self->do_auth(raw);
        else
            self->on_message(raw);
        self->do_read();
    });
}

// void WebSocketSession::do_auth(const std::string& raw) {
//     try {
//         auto j = json::parse(raw);
//         if (j["type"] != "auth") { close(); return; }
//         auto result = auth_.authenticate(j["token"].get<std::string>());
//         if (!result.ok) { close(); return; }
//         user_id_       = result.value.id;
//         authenticated_ = true;
//         connections_.register_session(user_id_, shared_from_this());
//         presence_.user_connected(user_id_);
//         do_write(json{{"type", "auth_ok"}, {"user_id", user_id_}}.dump());
//     } catch (...) { close(); }
// }

void WebSocketSession::do_auth(const std::string& raw) {
    try {
        auto j = json::parse(raw);
        std::string type = j["type"];

        if (type == "register") {
            auto res = auth_.register_user(
                j["username"].get<std::string>(),
                j["email"].get<std::string>(),
                j["password"].get<std::string>());
            if (!res.ok) { do_write(json{{"type", "error"}, {"message", res.message}}.dump()); return; }
            do_write(json{{"type", "register_ok"}}.dump());
            return;
        }

        if (type == "login") {
            auto res = auth_.login(
                j["email"].get<std::string>(),
                j["password"].get<std::string>());
            if (!res.ok) { do_write(json{{"type", "error"}, {"message", res.message}}.dump()); return; }
            do_write(json{{"type", "login_ok"}, {"token", res.value}}.dump());
            return;
        }

        if (type == "send_message") {
            auto res = messages_.send_message(user_id_,
                j["payload"]["conversation_id"], j["payload"]["content"]);
            if (!res.ok) {
                do_write(json{{"type", "error"}, {"message", res.message}}.dump());
            } else {
                do_write(json{{"type", "message_ack"}, {"status", "saved"}}.dump());
            }
        }

        if (type != "auth") { close(); return; }

        auto result = auth_.authenticate(j["token"].get<std::string>());
        if (!result.ok) { close(); return; }

        user_id_       = result.value.id;
        authenticated_ = true;
        connections_.register_session(user_id_, shared_from_this());
        presence_.user_connected(user_id_);
        do_write(json{{"type", "auth_ok"}, {"user_id", user_id_}}.dump());
    } catch (...) { close(); }
}

void WebSocketSession::on_message(const std::string& raw) {
    try {
        auto j    = json::parse(raw);
        std::string type = j["type"];

        if (type == "send_message") {
            auto res = messages_.send_message(user_id_,
                j["payload"]["conversation_id"], j["payload"]["content"]);
            if (!res.ok)
                do_write(json{{"type", "error"}, {"message", res.message}}.dump());
        } else if (type == "typing_start") {
            typing_.start_typing(user_id_, j["payload"]["conversation_id"]);
        } else if (type == "typing_stop") {
            typing_.stop_typing(user_id_, j["payload"]["conversation_id"]);
        } else if (type == "message_ack") {
            delivery_.acknowledge(j["payload"]["message_id"]);
        } else if (type == "message_read") {
            delivery_.mark_read(j["payload"]["message_id"]);
        } else if (type == "pong") {
            presence_.user_connected(user_id_); // refresh TTL
        }
    } catch (...) {
        do_write(json{{"type", "error"}, {"message", "invalid payload"}}.dump());
    }
}

void WebSocketSession::send(const std::string& payload) {
    do_write(payload);
}

void WebSocketSession::do_write(const std::string& payload) {
    auto self = shared_from_this();
    auto buf  = std::make_shared<std::string>(payload);
    ws_.async_write(net::buffer(*buf), [self, buf](beast::error_code ec, std::size_t) {
        if (ec) self->close();
    });
}

void WebSocketSession::do_ping() {
    auto self = shared_from_this();
    ws_.async_ping({}, [self](beast::error_code ec) {
        if (ec) self->close();
    });
}

void WebSocketSession::close() {
    if (!user_id_.empty()) {
        presence_.user_disconnected(user_id_);
        connections_.remove_session(user_id_);
    }
    beast::error_code ec;
    ws_.close(beast::websocket::close_code::normal, ec);
}

} // namespace chat
