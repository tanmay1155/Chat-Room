#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <mutex>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "12345"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
std::vector<SOCKET> clients;
std::mutex clientsMutex;
void handleClient(SOCKET clientSocket, int serialNumber) {
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::lock_guard<std::mutex> lock(clientsMutex);
            auto it = std::find(clients.begin(), clients.end(), clientSocket);
            if (it != clients.end()) {
                clients.erase(it);
            }
            std::cout << "Client " << serialNumber << " disconnected\n";
            closesocket(clientSocket);
            return;
        }
        buffer[bytesReceived] = '\0';
        std::cout << "Received from client " << serialNumber << ": " << buffer << std::endl;
        for (const auto& client : clients) {
            if (client != clientSocket) {
                send(client, buffer, bytesReceived, 0);
            }
        }
    }
}
void sendMessageToClient(int serialNumber) {
    std::string message;
    std::cout << "Enter message to send to client " << serialNumber << ": ";
    std::cin >> message;
    std::lock_guard<std::mutex> lock(clientsMutex);
    if (serialNumber > 0 && serialNumber <= clients.size()) {
        int index = serialNumber - 1;
        send(clients[index], message.c_str(), message.length(), 0);
    }
    else {
        std::cout << "Invalid serial number" << std::endl;
    }
}

int main() {
    WSADATA wsaData;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    int iResult;
    int clientCount = 0;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server listening on port " << DEFAULT_PORT << std::endl;
    while (true) {
        clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back(clientSocket);
        std::thread(handleClient, clientSocket, ++clientCount).detach();
    }
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
