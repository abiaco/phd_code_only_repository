
#ifndef ENOBIO_CONNECT_CPP
#define ENOBIO_CONNECT_CPP

#include "lock_state.hpp"
#include "constants.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

void enobio_connect(const char* port, LockState *s, bool *running, bool *recording, bool *typed){

    WSADATA wsaData;
    int iResult;

    SOCKET ConnSocket = INVALID_SOCKET;


    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[1];
    int recvbuflen = 1;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    //hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to server
    ConnSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = connect( ConnSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("connect failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ConnSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    char* sendbuff = "<trigger>10</trigger>";
    int len = 21;
    iResult = send(ConnSocket, sendbuff, len, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
    printf("Bytes Sent: %ld\n", iResult);

    cout << "enobio connected\n";
    
    s->setState(1);
    do {
        if (typed){
            iResult = send(ConnSocket, sendbuff, len, 0);

            if (iResult > 0) {
                typed = false;
            }
            else if (iResult == 0){
                cout << "connection closed Enobio\n";
                *running = false;
                s->setState(0);
            }
        }
        
    } while (*running);

    cout << "closing sockets on Enobio Conn\n";
    closesocket(ConnSocket);
    WSACleanup();
    cout << " ENOBIO DONE\n";
}

#endif