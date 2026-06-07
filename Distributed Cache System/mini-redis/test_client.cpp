#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

void sendCommand(SOCKET sock, const std::string& command) {
    send(sock, command.c_str(), command.length(), 0);
    
    char buffer[4096] = {0};
    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6379);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    std::cout << "====================================\n";
    std::cout << "  Connected to Valkeyrie Server!\n";
    std::cout << "====================================\n\n";
    
    // Test PING
    std::cout << ">>> PING\n";
    sendCommand(sock, "*1\r\n$4\r\nPING\r\n");
    std::cout << "\n";
    
    // Test SET
    std::cout << ">>> SET user:1 \"Alice\"\n";
    sendCommand(sock, "*3\r\n$3\r\nSET\r\n$6\r\nuser:1\r\n$5\r\nAlice\r\n");
    std::cout << "\n";
    
    // Test GET
    std::cout << ">>> GET user:1\n";
    sendCommand(sock, "*2\r\n$3\r\nGET\r\n$6\r\nuser:1\r\n");
    std::cout << "\n";
    
    // Test SET another key
    std::cout << ">>> SET user:2 \"Bob\"\n";
    sendCommand(sock, "*3\r\n$3\r\nSET\r\n$6\r\nuser:2\r\n$3\r\nBob\r\n");
    std::cout << "\n";
    
    // Test EXISTS
    std::cout << ">>> EXISTS user:1\n";
    sendCommand(sock, "*2\r\n$6\r\nEXISTS\r\n$6\r\nuser:1\r\n");
    std::cout << "\n";
    
    // Test DEL
    std::cout << ">>> DEL user:1\n";
    sendCommand(sock, "*2\r\n$3\r\nDEL\r\n$6\r\nuser:1\r\n");
    std::cout << "\n";
    
    // Test EXISTS after delete
    std::cout << ">>> EXISTS user:1 (after delete)\n";
    sendCommand(sock, "*2\r\n$6\r\nEXISTS\r\n$6\r\nuser:1\r\n");
    std::cout << "\n";
    
    // Test GET non-existent
    std::cout << ">>> GET user:1 (should be null)\n";
    sendCommand(sock, "*2\r\n$3\r\nGET\r\n$6\r\nuser:1\r\n");
    std::cout << "\n";
    
    // Test INFO
    std::cout << ">>> INFO\n";
    sendCommand(sock, "*1\r\n$4\r\nINFO\r\n");
    std::cout << "\n";
    
    std::cout << "====================================\n";
    std::cout << "  All tests completed!\n";
    std::cout << "====================================\n";
    
    closesocket(sock);
    WSACleanup();
    return 0;
}
