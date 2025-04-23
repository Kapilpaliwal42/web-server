#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Step 1: Initialize Winsock
    WSADATA wsaData;
    int wsInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsInit != 0) {
        std::cerr << "WSAStartup failed: " << wsInit << "\n";
        return 1;
    }

    // Step 2: Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Step 3: Bind the socket to a port
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accept from any IP
    serverAddr.sin_port = htons(8080);       // Port 8080

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Step 4: Listen for connections
    listen(serverSocket, SOMAXCONN);
    std::cout << "Server is listening on port 8080...\n";

    // Step 5: Accept a client
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Step 6: Receive data from the client
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        std::cout << "Received: " << std::string(buffer, bytesReceived) << "\n";

        // Step 7: Send a response
        std::string response = "Hello from server!\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    // Step 8: Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
