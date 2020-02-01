/*_____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____
 |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
  _     ____                   ____             _        _                ___   _        _
 | |   / ___|_ __ ___  ___ ___/ ___|  ___   ___| | _____| |_ ___  __   __/ _ \ / |      | |
 | |  | |   | '__/ _ \/ __/ __\___ \ / _ \ / __| |/ / _ \ __/ __| \ \ / / | | || |      | |
 | |  | |___| | | (_) \__ \__ \___) | (_) | (__|   <  __/ |_\__ \  \ V /| |_| || |      | |
 | |   \____|_|  \___/|___/___/____/ \___/ \___|_|\_\___|\__|___/   \_/  \___(_)_|      | |
 |_|                                                                                    |_|
  _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____
 |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|


=========================================================
    -Copyright (C) 2019 Neoa Software

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
=========================================================

*/
#include "CPSocket.h"

/*CPSocket();
~CPSocket();

void StartClient();
void StartServer();

bool IsConnected();
bool IsAuth();

void SendString();
std::string GetString();




private:
    std::thread SocketThreadRx;
    std::thread SocketThreadTx;
    Server ConnectionProp; 

    bool IsConnectedTx = false;
    bool IsConnectedRx = false;
    bool IsAuthTx = false;
    bool IsAuthRx = false;


    void ClientTx();
    void ClientRx();

    void ServerTx();
    void ServerRx();*/

#define AuthString "AUTH - SSC HyperSocketLink (COM METHOD START - SECURE SOCKET COMMUNICATION)"
#define bufferSize 1000000
#define ConnectionTryCount 10

// == CPSocket Methods
    // initing
CPSocket::CPSocket() { srand(time(0)); AUTH_NUMBER = (((rand() + rand()) / rand()) * rand() + rand() + rand()); }
CPSocket::CPSocket(std::string IP, int TxPort, int RxPort) { srand(time(0)); ConnectionProp.IP = IP; ConnectionProp.InBoundPort = RxPort; ConnectionProp.OutBound = TxPort; AUTH_NUMBER = (((rand() + rand()) / rand()) * rand() + rand() + rand());  }
CPSocket::~CPSocket() { 
    SocketThreadRx.join();
    SocketThreadTx.join();
}

    // starting servers
void CPSocket::StartClient() {
    if (!IsSocketsFull) {
        SocketThreadRx = std::thread(&CPSocket::ClientRx, this);
        SocketThreadTx = std::thread(&CPSocket::ClientTx, this);

        displayout(D_INFO, "Starting Socket Threads for Client");
        displayout(D_INFO, "Auth Number Created: \\");
        std::cout << AUTH_NUMBER << std::endl << std::endl;


        IsSocketsFull = true;
    }
}

void CPSocket::StartServer() {
    if (!IsSocketsFull) {
        std::thread XMPserverRx = std::thread(&CPSocket::ServerRx, this);
        std::thread XMPserverTx = std::thread(&CPSocket::ServerTx, this);

        ServerThreadsRx.push_back(move(XMPserverRx));

        displayout(D_INFO, "Starting Socket Threads for Client");
        displayout(D_INFO, "Auth Number Created: \\");
        std::cout << AUTH_NUMBER << std::endl << std::endl;

        IsSocketsFull = true;
    }
}

bool CPSocket::IsConnected() {
    return IsConnectedRx && IsConnectedTx;
}
bool CPSocket::IsAuth() {
    return IsAuthSSC;
}

void CPSocket::SendString(std::string RecStr) {
    TxQue.push_back(RecStr);
}
std::vector<std::string> CPSocket::GetString() {
    std::vector<std::string> SendQ = RxQue;
    RxQue.clear();
}

void CPSocket::ClientRx() {
RxStart:

    Sleep(120);
    displayout(D_INFO, "[RX] STARTING ...");
    displayout(D_INFO, "[RX] Starting Connection: %s::%d", ConnectionProp.IP, ConnectionProp.InBoundPort);

    displayout(D_INFO, "[RX] Windows Platform - Startig WinSock");

    WSAData Data;
    WORD ver = MAKEWORD(2, 2);

    int WS_START = WSAStartup(ver, &Data);
    if (WS_START != 0) {
        displayout(D_ERROR, "[RX] Unable To Start WinSock. ERROR #%d", WS_START);

        return;
    }

    // Create The Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        displayout(D_ERROR, "[RX] Can't create socket, Err #%s", WSAGetLastError());
        WSACleanup();
        return;
    }

    displayout(D_INFO, "[RX] Winsock Started!");
    displayout(D_INFO, "[RX] Starting hint structure...");
    
    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(ConnectionProp.InBoundPort);
    inet_pton(AF_INET, ConnectionProp.IP.c_str(), &hint.sin_addr);

    // Connect to server

    displayout(D_INFO, "[RX] Setup complete!");
    
    int TryCount = 0;
    do {
        displayout(D_INFO, "[RX] Conneting...");
        int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connResult == SOCKET_ERROR)
        {
            displayout(D_WARNING, "[RX] Connection Failed, Retrying...");
            TryCount++;
        }
        else {
            break;
        }
        if (TryCount >= ConnectionTryCount) {
            ThreadShouldStop = true;
            return;
        }
    } while (!ThreadShouldStop);

    displayout(D_INFO, "[RX] CONNECTED");
    IsConnectedRx = true;

#ifndef DISABLE_AUTH
    // Start Authing
    if (!ThreadShouldStop) {
        displayout(D_LOG, "[RX] Begining Auth...");


        while (!ThreadShouldStop) {
            std::string GetRsc = RxV(sock);
            displayout(D_INFO, GetRsc.c_str());


            if (GetRsc == AuthString) {
                displayout(D_INFO, "[RX] Got Init String ");
                IsAuthSSC = true;
                break;
            }
        }
    }
    
#endif // !DISABLE_AUTH

    while (!ThreadShouldStop) {
        std::string Rx = RxV(sock);
        if (Rx.size() > 0) {
            RxQue.push_back(Rx);
        }
    }

    if (ThreadShouldRestart) {
        ThreadShouldStop = false;
        ThreadShouldRestart = false;
        goto RxStart;
    }

    return;
}
void CPSocket::ClientTx() {
TxStart:

    Sleep(100);
    displayout(D_INFO, "[TX] STARTING ...");
    displayout(D_INFO, "[TX] Starting Connection: %s::%d", ConnectionProp.IP, ConnectionProp.InBoundPort);

    displayout(D_INFO, "[TX] Windows Platform - Startig WinSock");

    WSAData Data;
    WORD ver = MAKEWORD(2, 2);

    int WS_START = WSAStartup(ver, &Data);
    if (WS_START != 0) {
        displayout(D_ERROR, "[TX] Unable To Start WinSock. ERROR #%d", WS_START);

        return;
    }

    // Create The Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        displayout(D_ERROR, "[TX] Can't create socket, Err #%s", WSAGetLastError());
        WSACleanup();
        return;
    }

    displayout(D_INFO, "[TX] Winsock Started!");
    displayout(D_INFO, "[TX] Starting hint structure...");

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(ConnectionProp.InBoundPort);
    inet_pton(AF_INET, ConnectionProp.IP.c_str(), &hint.sin_addr);

    // Connect to server

    displayout(D_INFO, "[TX] Setup complete!");

    int TryCount = 0;
    do {
        displayout(D_INFO, "[TX] Conneting...");
        int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connResult == SOCKET_ERROR)
        {
            displayout(D_WARNING, "[TX] Connection Failed, Retrying...");
            TryCount++;
        }
        else {
            break;
        }
        if (TryCount >= ConnectionTryCount) {
            ThreadShouldStop = true;
            return;
        }
    } while (!ThreadShouldStop);

    displayout(D_INFO, "[TX] CONNECTED");
    IsConnectedRx = true;

#ifndef DISABLE_AUTH
    // Start Authing
    if (!ThreadShouldStop) {
        displayout(D_INFO, "[TX] Sending Auth");
        while (!ThreadShouldStop) {
            
            TxV(sock, AuthString);

            if (IsAuthSSC) {
                break;
            }
        }
    }

#endif // !DISABLE_AUTH

    while (!ThreadShouldStop) {
        if (TxQue.size() > 0) {
            TxV(sock, TxQue[0]);
            displayout(D_LOG, TxQue[0].c_str());
            TxQue.erase(TxQue.begin());
        }
    }

    if (ThreadShouldRestart) {
        ThreadShouldStop = false;
        ThreadShouldRestart = false;
        goto TxStart;
    }

    return;
}

void CPSocket::ServerRx() {
    Sleep(100);
}
void CPSocket::ServerTx() {
    Sleep(100);
}

std::string CPSocket::RxV(SOCKET sock) {
    char buf[bufferSize + 10];
    ZeroMemory(buf, bufferSize + 10);

    // Wait for client to send data
    int bytesReceived = recv(sock, buf, bufferSize + 10, 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        displayout(D_ERROR, "Error in recv, Quitting Thread...");
        ThreadShouldStop = true;
    }

    if (bytesReceived == 0)
    {
        displayout(D_WARNING, "Disconnected...");
        ThreadShouldStop = true;
    }
    ThreadShouldRestart = true;
    return std::string(buf);
}

void CPSocket::TxV(SOCKET COM, std::string Data) {
    send(COM, Data.c_str(), Data.length() + 1, 0);
}