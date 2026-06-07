#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#pragma comment(lib, "ws2_32.lib")

std::string createRESP(const std::vector<std::string>& parts) {
    std::string resp = "*" + std::to_string(parts.size()) + "\r\n";
    for (const auto& part : parts) {
        resp += "$" + std::to_string(part.length()) + "\r\n";
        resp += part + "\r\n";
    }
    return resp;
}

void sendCommand(SOCKET sock, const std::string& command, const std::string& cmdType = "") {
    send(sock, command.c_str(), command.length(), 0);
    
    char buffer[4096] = {0};
    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        
        std::string response(buffer);
        
        if (cmdType == "KEYS" && response[0] == '*') {
            std::cout << "Keys: ";
            std::istringstream iss(response.substr(response.find('\n') + 1));
            std::string line;
            std::vector<std::string> keys;
            
            while (std::getline(iss, line)) {
                if (line.empty() || line[0] == '$' || line == "\r") continue;
                if (line.back() == '\r') line.pop_back();
                if (!line.empty() && line[0] != '*') {
                    keys.push_back(line);
                }
            }
            
            if (keys.empty()) {
                std::cout << "(empty)";
            } else {
                for (size_t i = 0; i < keys.size(); i++) {
                    std::cout << keys[i];
                    if (i < keys.size() - 1) std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        else if (response[0] == '+') {
            std::cout << "Response: " << response.substr(1, response.find('\r') - 1) << std::endl;
        }
        else if (response[0] == ':') {
            std::cout << "Response: " << response.substr(1, response.find('\r') - 1) << std::endl;
        }
        else if (response[0] == '$') {
            size_t firstNewline = response.find('\n');
            if (firstNewline != std::string::npos) {
                std::string value = response.substr(firstNewline + 1);
                if (!value.empty() && value.back() == '\n') value.pop_back();
                if (!value.empty() && value.back() == '\r') value.pop_back();
                std::cout << "Response: " << value << std::endl;
            } else {
                std::cout << "Response: (null)" << std::endl;
            }
        }
        else {
            std::cout << "Response: " << buffer << std::endl;
        }
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6379);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cerr << "Failed to connect to Valkeyrie on 127.0.0.1:6379\n";
        std::cerr << "Make sure the server is running!\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════╗\n";
    std::cout << "║   Connected to Valkeyrie Server! ⚡      ║\n";
    std::cout << "║   Type 'help' for commands               ║\n";
    std::cout << "║   Type 'quit' to exit                    ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    std::string line;
    while (true) {
        std::cout << "valkeyrie> ";
        std::getline(std::cin, line);
        
        if (line.empty()) continue;
        
        if (line == "quit" || line == "exit") {
            std::cout << "Goodbye!\n";
            break;
        }
        
        if (line == "help") {
            std::cout << "\nAvailable Commands:\n";
            std::cout << "  PING                    - Test connection\n";
            std::cout << "  SET <key> <value>       - Set a key-value pair\n";
            std::cout << "  GET <key>               - Get value by key\n";
            std::cout << "  DEL <key>               - Delete a key\n";
            std::cout << "  EXISTS <key>            - Check if key exists\n";
            std::cout << "  EXPIRE <key> <seconds>  - Set expiration time\n";
            std::cout << "  INFO                    - Get cache statistics\n";
            std::cout << "  KEYS                    - List all keys\n";
            std::cout << "  GETALL                  - Show all key-value pairs\n";
            std::cout << "  help                    - Show this help\n";
            std::cout << "  quit                    - Exit client\n\n";
            std::cout << "Examples:\n";
            std::cout << "  SET user:1 Alice\n";
            std::cout << "  GET user:1\n";
            std::cout << "  GETALL\n\n";
            continue;
        }
        
        std::string upperLine = line;
        std::transform(upperLine.begin(), upperLine.end(), upperLine.begin(), ::toupper);
        
        if (upperLine == "GETALL") {
            std::string keysResp = createRESP({"KEYS"});
            send(sock, keysResp.c_str(), keysResp.length(), 0);
            
            char buffer[4096] = {0};
            int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
            
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                std::string response(buffer);
                
                std::istringstream iss(response.substr(response.find('\n') + 1));
                std::string keyLine;
                std::vector<std::string> keys;
                
                while (std::getline(iss, keyLine)) {
                    if (keyLine.empty() || keyLine[0] == '$' || keyLine == "\r") continue;
                    if (keyLine.back() == '\r') keyLine.pop_back();
                    if (!keyLine.empty() && keyLine[0] != '*') {
                        keys.push_back(keyLine);
                    }
                }
                
                if (keys.empty()) {
                    std::cout << "(empty - no keys stored)\n";
                } else {
                    std::cout << "\nAll Key-Value Pairs:\n";
                    std::cout << "=====================\n";
                    
                    for (const auto& key : keys) {
                        std::string getResp = createRESP({"GET", key});
                        send(sock, getResp.c_str(), getResp.length(), 0);
                        
                        char valBuffer[4096] = {0};
                        int valBytes = recv(sock, valBuffer, sizeof(valBuffer) - 1, 0);
                        
                        if (valBytes > 0) {
                            valBuffer[valBytes] = '\0';
                            std::string valResp(valBuffer);
                            
                            size_t firstNewline = valResp.find('\n');
                            if (firstNewline != std::string::npos) {
                                std::string value = valResp.substr(firstNewline + 1);
                                if (!value.empty() && value.back() == '\n') value.pop_back();
                                if (!value.empty() && value.back() == '\r') value.pop_back();
                                std::cout << "  " << key << " => " << value << "\n";
                            }
                        }
                    }
                    std::cout << "=====================\n";
                }
            }
            std::cout << std::endl;
            continue;
        }
        
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string part;
        while (iss >> part) {
            parts.push_back(part);
        }
        
        if (parts.empty()) continue;
        
        std::string resp = createRESP(parts);
        
        std::string cmdType = "";
        if (!parts.empty()) {
            std::string cmd = parts[0];
            std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
            cmdType = cmd;
        }
        
        sendCommand(sock, resp, cmdType);
        std::cout << std::endl;
    }
    
    closesocket(sock);
    WSACleanup();
    return 0;
}
