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

#define bufferSize 1000000

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
    return IsAuthRx && IsAuthTx;
}

void CPSocket::SendString(std::string RecStr) {
    TxQue.push_back(RecStr);
}
std::vector<std::string> CPSocket::GetString() {
    std::vector<std::string> SendQ = RxQue;
    RxQue.clear();
}

void CPSocket::ClientRx() {
    Sleep(100);
    displayout(D_INFO, "RX STARTING ...");
    displayout(D_INFO, "RX Starting Connection: %s::%d", ConnectionProp.IP, ConnectionProp.InBoundPort);

    displayout(D_INFO, "Windows Platform - Startig WinSock");

    WSAData Data;
    WORD ver = MAKEWORD(2, 2);

    int WS_START = WSAStartup(ver, &Data);
    if (WS_START != 0) {
        displayout(D_ERROR, "Unable To Start WinSock. ERROR #%d", WS_START);

        return;
    }

    // Create The Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        displayout(D_ERROR, "Can't create socket, Err #%s", WSAGetLastError());
        WSACleanup();
        return;
    }

    displayout(D_INFO, "Winsock Started!");
    displayout(D_INFO, "Starting hint structure...");
    
    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(ConnectionProp.InBoundPort);
    inet_pton(AF_INET, ConnectionProp.IP.c_str(), &hint.sin_addr);

    // Connect to server

    displayout(D_INFO, "Setup complete!");
    

    do {
        static int TryCount = 0;
        displayout(D_INFO, "Conneting...");
        int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connResult == SOCKET_ERROR)
        {
            displayout(D_WARNING, "Connection Failed, Retrying...");
            TryCount++;
        }
        else {
            break;
        }
        if (TryCount >= 5) {
            ThreadShouldStop = true;
            return;
        }
    } while (!ThreadShouldStop);


    // Start Authing
    if (!ThreadShouldStop) {
        // Do-while loop to send and receive data
        char buf[bufferSize];


        displayout(D_LOG, "CONNECTED");
        IsConnectedRx = true;

    
        displayout(D_LOG, "Begining Auth");

        
    }
    

    return;
}
void CPSocket::ClientTx() {
    Sleep(100);
}

void CPSocket::ServerRx() {
    Sleep(100);
}
void CPSocket::ServerTx() {
    Sleep(100);
}



