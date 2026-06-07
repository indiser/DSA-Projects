#ifndef SERVER_H
#define SERVER_H

#include "cache/CacheEngine.h"
#include "Protocol.h"
#include "utils/ThreadPool.h"
#include <stdexcept>
#include <string>
#include <memory>
#include <atomic>

#ifdef _WIN32
    #include <winsock2.h>
#endif

class Server {
private:
    std::unique_ptr<CacheEngine> cache;
    std::unique_ptr<Protocol> protocol;
    std::unique_ptr<ThreadPool> threadPool;
    
    std::string host;
    int port;
    std::atomic<bool> running;
    
#ifdef _WIN32
    SOCKET serverSocket;
#else
    int serverSocket;
#endif
    
    void handleClient(int clientSocket);
    std::string processCommand(const Protocol::Message& msg);
    
public:
    Server(const std::string& host, int port, size_t cacheCapacity, 
           EvictionPolicyType policy, size_t numThreads = 4);
    ~Server();
    
    void start();
    void stop();
    bool isRunning() const;
};

#endif // SERVER_H
