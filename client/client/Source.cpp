#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "12345"
#define BUFFER_SIZE 1024
int main() {
    WSADATA wsaData;
    SOCKET connectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    int iResult;
    char sendBuffer[BUFFER_SIZE];
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server" << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to server. Enter 'quit' to exit.\n";
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(sendBuffer, BUFFER_SIZE);

        if (_stricmp(sendBuffer, "quit") == 0) {
            break;
        }
        iResult = send(connectSocket, sendBuffer, strlen(sendBuffer), 0);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            closesocket(connectSocket);
            WSACleanup();
            return 1;
        }
    }
    closesocket(connectSocket);
    WSACleanup();
    return 0;
}
