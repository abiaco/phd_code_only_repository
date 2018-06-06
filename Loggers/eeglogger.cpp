#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <chrono>
#include <ctime>
#include <thread>
#include <iterator>
#include <map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <deque>


#include "../Utils/lock_state.hpp"
#include "../Utils/blocking_queue.hpp"
#include "../Utils/constants.h"
#include "../Utils/utils.hpp"


#include "../Utils/consumer.cpp"
#include "../Utils/gui_listen.cpp"

using namespace std;

int main() {
    b_queue<unsigned char*> q;

    LockState s;
    bool running = true;
    bool recording = true;
    thread guiThread(gui_connect, GUI_PORT, &s, &running, &recording);

    s.wait();

    cout << "done waiting; state: " << s.getState();
    WSADATA wsaData;
    SOCKET Socket = INVALID_SOCKET;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;


    char recvbuffer[BUFFER_LENGTH];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        cout <<"Winsock error - init failed";
        WSACleanup();
        return 0;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(hostname, ENOBIO_PORT, &hints, &result) != 0){
        cout <<"Winsock error - get addr info failed "<< WSAGetLastError();
        WSACleanup();
        return 0;
    }
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next){
        Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (Socket == INVALID_SOCKET){
            cout<<"socket failed with error: " << WSAGetLastError();
            WSACleanup();
            return 0;
        }

        if (connect(Socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR){
            closesocket(Socket);
            Socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (Socket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 0;
    }

    bool reading = false;
    bool conn = true;
    thread consumer(consume, &q, &conn);

    cout<< "connected\n";
    int iResult;

    do {

        iResult = recv(Socket, recvbuffer, BUFFER_LENGTH, MSG_WAITALL);

        cout << "received " << iResult << " bytes.\n";

        if (iResult > 0){
            if (!running){
                cout << "gui said stop\n";
                conn = false;
                cout << " closing socket main\n";
                closesocket(Socket);
                cout << " joining consumer\n";
                q.setStop();
                consumer.join();
                cout << " joining gui\n";
                guiThread.join();
                cout << "consumer joined. gui joined; breaking\n";
                WSACleanup();
                cout << "done";
                return 0;
            }
            if (s.getState() == STATE_RECORD) {
                if (!reading){
                    cout << "reading\n";
                    reading = true;
                }

                char temp_buff[BUFFER_LENGTH];
                memcpy(temp_buff, recvbuffer, BUFFER_LENGTH);
                unsigned char *cpy = new unsigned char[BUFFER_LENGTH];
                cpy = (unsigned char*)(temp_buff);
                //memcpy(cpy, recvbuffer, BUFFER_LENGTH);
                q.push(cpy);
            }
            else {
                if (reading) {
                    cout << "not reading\n";
                    reading = false;
                }
            }
        }
        else if (iResult == 0){

            cout << "connection closed in main";
            conn = false;
            running = false;
            q.setStop();
            consumer.join();
        }
        else
            cout << "recv failed with error: " << WSAGetLastError();
    } while (running);

    cout << "joining GUI";
    guiThread.join();
    cout << "cleaning up main";
    closesocket(Socket);
    WSACleanup();

    cout << "done!" << endl;
    return 0;
}

