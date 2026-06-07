#include "network/Server.h"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<Server> server;

void signalHandler(int signal) {
    std::cout << "\nShutting down Valkeyrie server..." << std::endl;
    if (server) {
        server->stop();
    }
    exit(signal);
}

int main(int argc, char* argv[]) {
    // Default configuration
    std::string host = "127.0.0.1";
    int port = 6379;
    size_t cacheCapacity = 10000;
    EvictionPolicyType policy = EvictionPolicyType::LRU;
    size_t numThreads = 4;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--host" && i + 1 < argc) {
            host = argv[++i];
        }
        else if (arg == "--port" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        }
        else if (arg == "--capacity" && i + 1 < argc) {
            cacheCapacity = std::stoul(argv[++i]);
        }
        else if (arg == "--policy" && i + 1 < argc) {
            std::string policyStr = argv[++i];
            if (policyStr == "LFU" || policyStr == "lfu") {
                policy = EvictionPolicyType::LFU;
            }
        }
        else if (arg == "--threads" && i + 1 < argc) {
            numThreads = std::stoul(argv[++i]);
        }
        else if (arg == "--help" || arg == "-h") {
            std::cout << "Valkeyrie - Distributed Cache System\n\n";
            std::cout << "Usage: valkeyrie [OPTIONS]\n\n";
            std::cout << "Options:\n";
            std::cout << "  --host <address>      Server host address (default: 127.0.0.1)\n";
            std::cout << "  --port <number>       Server port (default: 6379)\n";
            std::cout << "  --capacity <number>   Cache capacity (default: 10000)\n";
            std::cout << "  --policy <LRU|LFU>    Eviction policy (default: LRU)\n";
            std::cout << "  --threads <number>    Number of worker threads (default: 4)\n";
            std::cout << "  --help, -h            Show this help message\n\n";
            std::cout << "Examples:\n";
            std::cout << "  valkeyrie\n";
            std::cout << "  valkeyrie --port 8080 --capacity 5000\n";
            std::cout << "  valkeyrie --policy LFU --threads 8\n\n";
            return 0;
        }
    }
    
    // Display banner
    std::cout << R"(
╦  ╦┌─┐┬  ┬┌─┌─┐┬ ┬┬─┐┬┌─┐
╚╗╔╝├─┤│  ├┴┐├┤ └┬┘├┬┘│├┤ 
 ╚╝ ┴ ┴┴─┘┴ ┴└─┘ ┴ ┴└─┴└─┘
    )" << std::endl;
    
    std::cout << "⚡ Valkeyrie - Where keys meet their destiny\n" << std::endl;
    std::cout << "Configuration:" << std::endl;
    std::cout << "  Host:           " << host << std::endl;
    std::cout << "  Port:           " << port << std::endl;
    std::cout << "  Capacity:       " << cacheCapacity << " entries" << std::endl;
    std::cout << "  Eviction:       " << (policy == EvictionPolicyType::LRU ? "LRU" : "LFU") << std::endl;
    std::cout << "  Threads:        " << numThreads << std::endl;
    std::cout << std::endl;
    
    // Register signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    try {
        // Create and start server
        server = std::make_unique<Server>(host, port, cacheCapacity, policy, numThreads);
        server->start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
