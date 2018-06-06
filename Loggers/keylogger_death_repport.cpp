
#include "../Utils/gui_listen.cpp"
#include "../Utils/lock_state.hpp"
#include "../Utils/constants.h"


#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include "../Utils/utils.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

#define	DEBUG	1


#define CLASSNAME	"winkey"
#define WINDOWTITLE	"svchost"


using namespace std;

char	windir[MAX_PATH + 1];
HHOOK	kbdhook;	/* Keyboard hook handle */
HHOOK   moosehook; /* moose hook handle */
bool	running;	/* Used in main loop */
POINT point;

ofstream keyfile;
ofstream mousefile;
bool recording;
WSADATA wsaData;
int iResult;

SOCKET ConnSocket = INVALID_SOCKET;


struct addrinfo *result = NULL;
struct addrinfo hints;

int iSendResult;
char deathtrigger[21] = "<TRIGGER>5</TRIGGER>";
char synctrigger[22] = "<TRIGGER>10</TRIGGER>";
char summtrigger[21] = "<TRIGGER>6</TRIGGER>";
int len = 20;

// Initialize Winsock

/**
 * \brief Called by Windows automagically every time a key is pressed (regardless
 * of who has focus)
 */
__declspec(dllexport) LRESULT CALLBACK handlekeys(int code, WPARAM wp, LPARAM lp)
{
    if (!recording) return CallNextHookEx(kbdhook, code, wp, lp);
    if (code == HC_ACTION && (wp == WM_SYSKEYDOWN || wp == WM_KEYDOWN)) {

        char tmp[0xFF] = {0};
        string str;
        DWORD msg = 1;
        KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);

        /*
         * Get key name as string
         */
        msg += (st_hook.scanCode << 16);
        msg += (st_hook.flags << 24);
        GetKeyNameText(msg, tmp, 0xFF);
        str = string(tmp);
        if (str == "k" || str == "K"){
            cout << "sending death flag to enobio";
            iResult = send(ConnSocket, deathtrigger, len, 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnSocket);
                WSACleanup();
            }
            
        }
        if (str == "z" || str == "Z"){
            cout << "sending sync flag to enobio";
            iResult = send(ConnSocket, synctrigger, len + 1, 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnSocket);
                WSACleanup();
            }
        }

        if (str == "f" || str == "F" || str == "d" || str == "D"){
            cout << "sending summoner flag to enobio";
            iResult = send(ConnSocket, summtrigger, len, 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnSocket);
                WSACleanup();
            }
        }
        cout << "flag sent";

//        string path = OUTFILE_NAME;
//        ofstream outfile(path.c_str(), ios_base::app);
//        outfile << str << endl;
//        outfile.flush();
//        outfile.close();
        char stime[9];
        _strtime_s(stime, 9);
        keyfile << str << "," << stime << endl;
    }

    return CallNextHookEx(kbdhook, code, wp, lp);
}

__declspec(dllexport) LRESULT CALLBACK handlemoose(int code, WPARAM wp, LPARAM lp)
{

//    string path = MOOSE_OUTFILE_NAME;
//    ofstream outfile(path.c_str(), ios_base::app);
    if (!recording) return CallNextHookEx(moosehook, code, wp, lp);
    string mesg = "";
    if (wp == WM_LBUTTONDOWN){
        mesg = "LEFT_PRESS";
    }
    if (wp == WM_LBUTTONUP){
        mesg = "LEFT_RELEASE";
    }
    if (wp == WM_RBUTTONDOWN){
        mesg = "RIGHT_PRESS";
    }
    if (wp == WM_RBUTTONUP){
        mesg = "RIGHT_RELEASE";
    }
    if (wp == WM_MOUSEMOVE) {
        mesg = "MOVE";
    }

    GetCursorPos(&point);
    char stime[9];
    _strtime_s(stime, 9);
    mousefile << mesg << "," << point.x << "," << point.y << "," << stime << endl;


    return CallNextHookEx(moosehook, code, wp, lp);
}


/**
 * \brief Called by DispatchMessage() to handle messages
 * \param hwnd	Window handle
 * \param msg	Message to handle
 * \param wp
 * \param lp
 * \return 0 on success
 */
LRESULT CALLBACK windowprocedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
        case WM_CLOSE: case WM_DESTROY:
            running = false;
            break;
        default:
            /* Call default message handler */
            return DefWindowProc(hwnd, msg, wp, lp);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE thisinstance, HINSTANCE previnstance,
                   LPSTR cmdline, int ncmdshow)
{
    /*
     * Set up window
     */
    HWND		hwnd;
    HWND		fgwindow = GetForegroundWindow(); /* Current foreground window */
    MSG		    msg;
    WNDCLASSEX	windowclass;
    HINSTANCE	modulehandle;
    HINSTANCE   moosehandle;

    windowclass.hInstance = thisinstance;
    windowclass.lpszClassName = CLASSNAME;
    windowclass.lpfnWndProc = windowprocedure;
    windowclass.style = CS_DBLCLKS;
    windowclass.cbSize = sizeof(WNDCLASSEX);
    windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    windowclass.hCursor  = LoadCursor(NULL, IDC_ARROW);
    windowclass.lpszMenuName = NULL;
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = 0;
    windowclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (!(RegisterClassEx(&windowclass)))
        return 1;

    hwnd = CreateWindowEx(NULL, CLASSNAME, WINDOWTITLE, WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, HWND_DESKTOP, NULL,
                          thisinstance, NULL);
    if (!(hwnd))
        return 1;

    // setup the socket for the enobio connection
    cout << "setting up socket for enobio\n";
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    //hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, ENOBIO_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ConnSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = connect( ConnSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("connect failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ConnSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    cout << "enobio socket set up with no errors\n";
    /*
     * Make the window invisible
     */
#ifdef DEBUG
    /*
     * Debug mode: Make the window visible
     */
    ShowWindow(hwnd, SW_SHOW);
#else
    ShowWindow(hwnd, SW_HIDE);
#endif


    UpdateWindow(hwnd);
    SetForegroundWindow(fgwindow); /* Give focus to the previous fg window */

    cout << "generating files\n";

    string keyfile_name = generate_filename("Key");
    string mousefile_name = generate_filename("Mouse");


    cout << " opening files\n";
    keyfile.open(keyfile_name);
    mousefile.open(mousefile_name);
    cout <<"setting hooks\n";
    /*
     * Hook keyboard input so we get it too
     */
    recording = false;
    running = true;

    modulehandle = GetModuleHandle(NULL);
    moosehandle =  GetModuleHandle(NULL);
    kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)handlekeys, modulehandle, NULL);
    moosehook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)handlemoose, moosehandle, NULL);


    GetWindowsDirectory((LPSTR)windir, MAX_PATH);
    LockState s;

    /*
     * Main loop
     */

    thread guiThread(gui_connect, GUI_PORT_KEYLOGGER, &s, &running, &recording);
    cout << "waiting for GUI connection\n";
    s.wait();

    cout << "done waiting; state: " << s.getState();
    cout << " waiting for start\n";
    while (running) {
        /*
         * Get messages, dispatch to window procedure
         */
        if (!GetMessage(&msg, NULL, 0, 0))
            running = false; /*
					  * This is not a "return" or
					  * "break" so the rest of the loop is
					  * done. This way, we never miss keys
					  * when destroyed but we still exit.
					  */

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    cout << " not runnig. closing files\n";
    guiThread.join();
    UnhookWindowsHookEx(moosehook);
    UnhookWindowsHookEx(kbdhook);
    keyfile.flush();
    keyfile.close();
    mousefile.flush();
    mousefile.close();
    cout << "closing sockets on Enobio Conn\n";
    closesocket(ConnSocket);
    WSACleanup();
    cout << " ENOBIO DONE\n";
    return 0;
}