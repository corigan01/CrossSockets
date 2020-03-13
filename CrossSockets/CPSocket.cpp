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


#define AuthString "AUTH - SSC HyperSocketLink (COM METHOD START - SECURE SOCKET COMMUNICATION)"
#define bufferSize 1000000
#define ConnectionTryCount 10

#define DISABLE_AUTH 0

// == CPSocket Methods
    // initing
CPSocket::CPSocket() { srand(time(0)); AUTH_NUMBER = (((rand() + rand()) / rand()) * rand() + rand() + rand()); }
CPSocket::CPSocket(std::string IP, int TxPort, int RxPort) { srand(time(0)); ConnectionProp.IP = IP; ConnectionProp.InBoundPort = RxPort; ConnectionProp.OutBound = TxPort; AUTH_NUMBER = (((rand() + rand()) / rand()) * rand() + rand() + rand());  }
CPSocket::~CPSocket() { 
    ThreadShouldStop = true;
    ThreadShouldRestart = false;

    Sleep(100);

    bool SocketThreadRxDone = false;
    bool SocketThreadTxDone = false;
    

    displayout(D_WARNING, "CPSocket Closing...");

    while (!SocketThreadRxDone && !SocketThreadTxDone && AliveServers.size() != 0) {
        
        if (SocketThreadRx.joinable()) {
            SocketThreadRx.join();
            SocketThreadRxDone = true;
            displayout(D_INFO, "[RX] Joined Main");
        }
        if (SocketThreadTx.joinable()) {
            SocketThreadTx.join();
            SocketThreadTxDone = true;
            displayout(D_INFO, "[TX] Joined Main");
        }

        
    }
    
    displayout(D_WARNING, "CPSocket Closed!");
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
    ServerManagerThread = std::thread(&CPSocket::ServerManager, this);
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

    Sleep(1000);

    displayout(D_INFO, "[RX] CONNECTED");
    IsConnectedRx = true;

#ifndef DISABLE_AUTH
    // Start Authing
    if (!ThreadShouldStop) {
        displayout(D_INFO, "[RX] Begining Auth...");
            

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

    displayout(D_INFO, "[TX] Windows Platform - Using WinSock");

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

    Sleep(1000);

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
    displayout(D_INFO, "[TX] Done Auth");
#endif // !DISABLE_AUTH
    
    while (!ThreadShouldStop) {
        if (TxQue.size() > 0) {
            TxV(sock, AuthString);
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

void CPSocket::ServerManager() {
    displayout(D_INFO, "Server Manager Started!");
    while (!ThreadShouldStop || ThreadShouldRestart) {
        if (ServersFull) {
            displayout(D_INFO, "Spawning New Server");
            srand(rand() * time(0));
            int ClientId = (rand() * rand() / rand() + (rand() * rand() + rand() + rand()) / rand()) * rand() / rand();
            std::thread CopyThreadRX(&CPSocket::ServerRx, this, ClientId);
            std::thread CopyThreadTX(&CPSocket::ServerTx, this, ClientId);
            AliveServers.push_back(ClientId);
            ServerThreadsRx.push_back(std::move(CopyThreadRX));
            ServerThreadsTx.push_back(std::move(CopyThreadTX));
            HostNames.push_back("NO CONNECTION");

            ServerConnectionCount++;
            ServersFull = false;
            displayout(D_INFO, ("Connection Count: " + std::to_string(ServerConnectionCount)).c_str());
        }

        if (ServerActivity) {
            if (ServerThreadsRx.size() == ServerThreadsTx.size()) {
                for (int i = 0; i < ServerThreadsRx.size(); i++) {
                    if (AliveServers[i] == -1 && ServerThreadsRx[i].joinable() && ServerThreadsTx[i].joinable()) {
                        displayout(D_INFO, "Deleting Old Server...");

                        ServerThreadsRx[i].join();
                        ServerThreadsTx[i].join();

                        ServerThreadsRx.erase(ServerThreadsRx.begin() + i);
                        ServerThreadsTx.erase(ServerThreadsTx.begin() + i);

                        AliveServers.erase(AliveServers.begin() + i);
                        HostNames.erase(HostNames.begin() + i);

                        ServerConnectionCount--;
                        displayout(D_INFO, ("New Connection Count: " + std::to_string(ServerConnectionCount)).c_str());

                        ThreadShouldStop = false;
                        ThreadShouldRestart = false;
                    }
                }
            }
            else {
                displayout(D_ERROR, "Thread Mix, Thread Counts do not match");
                displayout(D_WARNING, "Restarting Threads...");

                AliveServers.empty();
                HostNames.empty();
                ServerConnectionCount = 0;
                ServersFull = true;

                for (int i = 0; i < ServerThreadsRx.size(); i++) {
                    ServerThreadsRx[i].join();
                }
                for (int i = 0; i < ServerThreadsTx.size(); i++) {
                    ServerThreadsTx[i].join();
                }

                ThreadShouldStop = false;
                ThreadShouldRestart = false;
            }
            ServerActivity = false;
        }
    }

    if (ServerConnectionCount > 0) {
        displayout(D_WARNING, "Killing all Servers");
    }

    AliveServers.empty();
    HostNames.empty();
    ServerConnectionCount = 0;
    ServersFull = true;

    for (int i = 0; i < ServerThreadsRx.size(); i++) {
        ServerThreadsRx[i].join();
    }
    for (int i = 0; i < ServerThreadsTx.size(); i++) {
        ServerThreadsTx[i].join();
    }
    
}

void CPSocket::ServerRx(int id) {
    Sleep(100);

    displayout(D_INFO, "[RX] STARTING ...");
    displayout(D_INFO, "[RX] Starting Connection: %s::%d", ConnectionProp.IP, ConnectionProp.InBoundPort);

    displayout(D_INFO, "[RX] Windows Platform - Using WinSock");

    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        displayout(D_ERROR, "[RX] Unable To Start WinSock. ERROR #%d");

    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        displayout(D_ERROR, "[RX] Unable To Start WinSock. ERROR #%d");

    }

    displayout(D_INFO, "[RX] Winsock Started!");
    displayout(D_INFO, "[RX] Starting hint structure...");

    // txtd the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(ConnectionProp.InBoundPort);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening 
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    displayout(D_INFO, "[TX] Setup complete!");

    displayout(D_INFO, "[TX] Conneting...");

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];		// Client's remote name
    char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);



    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        displayout(D_INFO, (host + std::string(" Connected on Port: ") + service).c_str());
        HostNames[LookUpArrayId(id)] = host;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        displayout(D_INFO, (host + std::string(" Connected on Port: ") + std::to_string(ntohs(client.sin_port))).c_str());
    }

    displayout(D_INFO, "[RX] CONNECTED");

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo message back to client


    ServersFull = true;

#ifndef DISABLE_AUTH
    // Start Authing
    if (!ThreadShouldStop) {
        displayout(D_INFO, "[RX] Begining Auth...");


        while (!ThreadShouldStop) {
            std::string GetRsc = RxV(clientSocket);
            displayout(D_INFO, GetRsc.c_str());


            if (GetRsc == AuthString) {
                displayout(D_INFO, "[RX] Got Init String ");
                IsAuthSSC = true;
                break;
            }
        }
    }

#endif // !DISABLE_AUTH

    while (!ThreadShouldStop)
    {
        std::string GetString = RxV(clientSocket);
        displayout(D_INFO, GetString.c_str());
        
        if (GetString.size() > 0) {
            RxQue.push_back(GetString);
        }

    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();


    AliveServers[LookUpArrayId(id)] = -1;
    ServerActivity = true;


}
void CPSocket::ServerTx(int id) {
    Sleep(115);

    displayout(D_INFO, "[TX] STARTING ...");
    displayout(D_INFO, "[TX] Starting Connection: %s::%d", ConnectionProp.IP, ConnectionProp.InBoundPort);

    displayout(D_INFO, "[TX] Windows Platform - Using WinSock");

    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        displayout(D_ERROR, "[TX] Unable To Start WinSock. ERROR #%d");

    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        displayout(D_ERROR, "[TX] Unable To Start WinSock. ERROR #%d");

    }
    displayout(D_INFO, "[TX] Winsock Started!");
    displayout(D_INFO, "[TX] Starting hint structure...");


    // txtd the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(ConnectionProp.OutBound);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 


    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening 
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    displayout(D_INFO, "[TX] Setup complete!");

    displayout(D_INFO, "[TX] Conneting...");

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);


    char host[NI_MAXHOST];		// Client's remote name
    char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);


    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        displayout(D_INFO, (host + std::string(" Connected on Port: ") + service).c_str());
        HostNames[LookUpArrayId(id)] = host;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        displayout(D_INFO, (host + std::string(" Connected on Port: ") + std::to_string(ntohs(client.sin_port))).c_str());
    }

    displayout(D_INFO, "[TX] CONNECTED");

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo message back to client


    ServersFull = true;

#ifndef DISABLE_AUTH
    // Start Authing
    if (!ThreadShouldStop) {
        displayout(D_INFO, "[TX] Begining Auth...");


        while (!ThreadShouldStop) {
            TxV(clientSocket, AuthString);

            
            if (IsAuthSSC) {
                break;
            }
        }
    }

#endif // !DISABLE_AUTH

    while (!ThreadShouldStop)
    {
        if (TxQue.size() > 0) {
            TxV(clientSocket, TxQue[0]);
            displayout(D_LOG, TxQue[0].c_str());
            TxQue.erase(TxQue.begin());
        }
    }

    // Close the socket
    closesocket(clientSocket);

    
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
    else {
        //displayout(D_DEBUG, std::to_string(bytesReceived).c_str());
    }
    ThreadShouldRestart = true;
    return std::string(buf);
}

void CPSocket::TxV(SOCKET COM, std::string Data) {
    send(COM, Data.c_str(), Data.length() + 1, 0);
}
