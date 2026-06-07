#include "network/Server.h"
#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

Server::Server(const std::string& host, int port, size_t cacheCapacity, 
               EvictionPolicyType policy, size_t numThreads)
    : host(host), port(port), running(false), serverSocket(INVALID_SOCKET) {
    
    cache = std::make_unique<CacheEngine>(cacheCapacity, policy);
    protocol = std::make_unique<Protocol>();
    threadPool = std::make_unique<ThreadPool>(numThreads);
    
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
#endif
}

Server::~Server() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void Server::start() {
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }
    
    // Set socket options
    int opt = 1;
#ifdef _WIN32
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
    
    // Bind socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(serverSocket);
        throw std::runtime_error("Failed to bind socket");
    }
    
    // Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(serverSocket);
        throw std::runtime_error("Failed to listen on socket");
    }
    
    running = true;
    std::cout << "Valkeyrie server listening on " << host << ":" << port << std::endl;
    
    // Accept connections
    while (running) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        
#ifdef _WIN32
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
#else
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
#endif
        
        if (clientSocket == INVALID_SOCKET) {
            if (running) {
                std::cerr << "Failed to accept connection" << std::endl;
            }
            continue;
        }
        
        // Handle client in thread pool
        threadPool->enqueue([this, clientSocket]() {
            handleClient(clientSocket);
        });
    }
}

void Server::stop() {
    running = false;
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
}

bool Server::isRunning() const {
    return running;
}

void Server::handleClient(int clientSocket) {
    char buffer[4096];
    
    while (running) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesReceived <= 0) {
            break;
        }
        
        buffer[bytesReceived] = '\0';
        std::string request(buffer);
        
        // Parse request
        Protocol::Message msg = protocol->parse(request);
        
        // Process command
        std::string response = processCommand(msg);
        
        // Send response
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    
    closesocket(clientSocket);
}

std::string Server::processCommand(const Protocol::Message& msg) {
    if (msg.type != Protocol::Type::Array || msg.array.empty()) {
        Protocol::Message errorMsg;
        errorMsg.type = Protocol::Type::Error;
        errorMsg.value = "ERR invalid command";
        return protocol->serialize(errorMsg);
    }
    
    std::string command = msg.array[0];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);
    
    Protocol::Message response;
    
    if (command == "GET" && msg.array.size() == 2) {
        auto value = cache->get(msg.array[1]);
        if (value.has_value()) {
            response.type = Protocol::Type::BulkString;
            response.value = value.value();
        } else {
            response.type = Protocol::Type::BulkString;
            response.value = "";
        }
    }
    else if (command == "SET" && msg.array.size() == 3) {
        cache->put(msg.array[1], msg.array[2]);
        response.type = Protocol::Type::SimpleString;
        response.value = "OK";
    }
    else if (command == "DEL" && msg.array.size() >= 2) {
        int deleted = 0;
        for (size_t i = 1; i < msg.array.size(); i++) {
            if (cache->remove(msg.array[i])) {
                deleted++;
            }
        }
        response.type = Protocol::Type::Integer;
        response.value = std::to_string(deleted);
    }
    else if (command == "EXISTS" && msg.array.size() == 2) {
        bool exists = cache->exists(msg.array[1]);
        response.type = Protocol::Type::Integer;
        response.value = exists ? "1" : "0";
    }
    else if (command == "EXPIRE" && msg.array.size() == 3) {
        try {
            int seconds = std::stoi(msg.array[2]);
            cache->setTTL(msg.array[1], seconds);
            response.type = Protocol::Type::Integer;
            response.value = "1";
        } catch (...) {
            response.type = Protocol::Type::Error;
            response.value = "ERR invalid expire time";
        }
    }
    else if (command == "PING") {
        response.type = Protocol::Type::SimpleString;
        response.value = "PONG";
    }
    else if (command == "INFO") {
        auto stats = cache->getStats();
        std::ostringstream oss;
        oss << "hits:" << stats.hits << "\r\n"
            << "misses:" << stats.misses << "\r\n"
            << "hit_rate:" << stats.hitRate() << "%\r\n"
            << "size:" << stats.size;
        response.type = Protocol::Type::BulkString;
        response.value = oss.str();
    }
    else if (command == "KEYS") {
        // Get all keys from storage
        auto allKeys = cache->getAllKeys();
        response.type = Protocol::Type::Array;
        response.array = allKeys;
    }
    else {
        response.type = Protocol::Type::Error;
        response.value = "ERR unknown command '" + command + "'";
    }
    
    return protocol->serialize(response);
}
