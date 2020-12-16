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

#pragma once

// ---------------- CODE ---------------- \\

#include "Base.h"
#include "CS_Server.h"
#include "CS_COM.h"

CS_Server::CS_Server(int port, bool AddBuildBack, bool htmlHost) {
    //ConnectionProp.IP = ip;
    stringBuildback = AddBuildBack;
    HtmlHost = htmlHost;
    ConnectionProp.OutBound = port;

    dis.out(D_INFO, "Server Starting...");
    dis.out(D_INFO, "PORT = " + std::to_string(port));

    SMT = std::thread(&CS_Server::ServerManager, this);
}

CS_Server::~CS_Server() {
    StopAll = true;
    
    SMT.join();
}

std::vector<std::string> CS_Server::GetAllClientNames() {
    std::vector<std::string> Ve;
    for (auto i : ServerQue) {
        if (i.Name != "NO CONNECTION") {
            Ve.push_back(i.Name);
        }
    }
    return Ve;
}

std::vector<ServVect>* CS_Server::GetAllClientSerVect() {
    std::vector<ServVect> Ve;
    for (auto i : ServerQue) {
        if (i.Name != "NO CONNECTION") {
            Ve.push_back(i);
        }
    }
    return &Ve;
}

ServVect* CS_Server::GetClientSerVect(std::string Name) {
    ServVect Ve;
    for (auto i : ServerQue) {
        if (i.Name == Name) {
            Ve = i;
            break;
        }
    }
    return &Ve;
}

void CS_Server::SendToClient(std::string Name, std::string Send) {
    for (auto& i : ServerQue) {
        if (i.Name == Name)
            i.SendingQue.push_back(Send);
    }
}

void CS_Server::SendToAll(std::string Send) {
    for (auto& i : ServerQue) {
        if (i.Name != "NO CONNECTION")
            i.SendingQue.push_back(Send);
    }

}

void CS_Server::HostHtml(std::string Html) {

    HtmlF = Html;

}

void CS_Server::DisconnectClient(std::string Name) {
    for (auto& i : ServerQue) {
        if (i.Name == Name) {
            i.shutdown = true;
        }
    }
}

void CS_Server::DisconnectAll() {
    for (auto& i : ServerQue) {
        i.shutdown = true;
    }
}

void CS_Server::SetConnections(bool connect) {
    Disconnect = connect;
}


void CS_Server::ServerManager() {
    /*if(!HtmlHost)*/ dis.out(D_INFO, "Server Manager has started!");

    while (!StopAll) {
        sleep_for(33ms);

        if (ServersFull && !Disconnect) {
            /*if(!HtmlHost)*/ dis.out(D_INFO, "Spawning New Thread Pool");
            srand(time(0));
            test:
            int ClientId = time(0) * rand() % rand() * rand();
            for (auto id : SocketIds) {
                if (ClientId == id) {
                    dis.out(D_WARNING, "Needed new Id");
                    goto test;
                }
            }
            std::thread CopyThread(&CS_Server::ServerThread, this, ClientId);
            ThreadPool.push_back(std::move(CopyThread));
            SocketIds.push_back(ClientId);
            ServerQue.push_back({ "NO CONNECTION" });

            ConnectionCount++;
            ServersFull = false;
            /*if(!HtmlHost)*/ dis.out(D_INFO, "New Connection Count = " + std::to_string(ConnectionCount - 1));
        }
        if (1) {
            //dis.out(D_DEBUG, std::to_string(ThreadPool.size()));
            for (int i = 0; i < ThreadPool.size(); i++) {
                if (SocketIds[i] == -1 && ThreadPool[i].joinable()) {
                    /*if(!HtmlHost)*/ dis.out(D_INFO, "Deleting Old Server...");
                    
                    ThreadPool[i].join();
                    
                    ThreadPool.erase(ThreadPool.begin() + i);
                    SocketIds.erase(SocketIds.begin() + i);
                    ServerQue.erase(ServerQue.begin() + i);
                    
                    ConnectionCount--;

                    /*if(!HtmlHost)*/ dis.out(D_INFO, "New Connection Count = " + std::to_string(ConnectionCount - 1));
                }
            }
            ServerActivity = false;
        }
        if (HtmlHost) {

            std::string HtmlHeader = R"(HTTP/1.0 200 OK
Date: Thu, 06 Aug 1998 12:00:15 GMT
Server: CrossSockets/0.1.5 (Unix)
Last-Modified: Mon, 22 Jun 1998
Content-Length: )" + std::to_string(HtmlF.size()) + R"(
Content-Type: text/html)";
            
            for (int i = 0; i < ServerQue.size(); i++) {
                if (ServerQue[i].Name != "NO CONNECTION")
                    ServerQue[i].SendingQue.push_back(HtmlHeader + "\n\n" + HtmlF);
            }
                
        }
    }
}



void CS_Server::ServerThread(int id) {
    Sleep(100);

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " STARTING ...");
    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Starting Connection: %s::%d");

#ifdef _WIN32

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Windows Platform - Using WinSock");

    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        dis.out(D_ERROR, std::to_string(id) + " Unable To Start WinSock. ERROR #%d");

    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        dis.out(D_ERROR, std::to_string(id) + " Unable To Start WinSock. ERROR #%d");

    }

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Winsock Started!");
    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Starting hint structure...");

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

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Setup complete!");

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Conneting...");

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];		// Client's remote name
    char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);



    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        /*if(!HtmlHost)*/ dis.out(D_INFO, (host + std::string(" Connected on Port: ") + service).c_str());
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        /*if(!HtmlHost)*/ dis.out(D_INFO, (host + std::string(" Connected on Port: ") + std::to_string(ntohs(client.sin_port))).c_str());
    }

    

    // Close listening socket
    closesocket(listening);
#endif

#ifdef __linux
    
    int server_fd, clientSocket, valread; 
    struct sockaddr_in address; 
    
    
    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Linux Platform - Using sys/socket");
    
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        dis.out(D_ERROR, std::to_string(id) + " Unable To Start sys/socket. ERROR #%d");
        exit(EXIT_FAILURE); 
    } 
       
    int opt = 1; 
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        dis.out(D_ERROR, std::to_string(id) + " Unable To Start sys/socket. ERROR #%d");
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( ConnectionProp.OutBound ); 
       
    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " sys/socket Started!");
    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Starting hint structure...");

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        dis.out(D_ERROR, std::to_string(id) + " BIND FAILED ERROR #%d");
        exit(EXIT_FAILURE); 
    }

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Setup done!");

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " Setting up listen..."); 

    if (listen(server_fd, 1) < 0) 
    { 
        dis.out(D_ERROR, std::to_string(id) + " Listen failed ERROR #%d");
        StopAll = true;
    } 

    dis.out(D_INFO, std::to_string(id) + " Connecting ...");

    int addrlen = sizeof(address); 
    if ((clientSocket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        /*if(!HtmlHost)*/ dis.out(D_ERROR, std::to_string(id) + " Failed to connect!"); 
        StopAll = true;
    } 

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + "Connected To client!"); 
#endif
    // While loop: accept and echo message back to client
    ServerQue[LookUpArrayId(id)].Name = std::to_string(id) + std::string("Test client");

    /*if(!HtmlHost)*/ dis.out(D_INFO, std::to_string(id) + " CONNECTED");


    ServersFull = true;

    CS_COM com(&clientSocket, HtmlHost);

    bool kill = false;

    while (!kill && !StopAll) {
        sleep_for(33ms);

        if (com.Error()) {
            if (!HtmlHost) dis.out(D_ERROR, std::to_string(id) + " COM ERROR");
            break;
        }

        int index = LookUpArrayId(id);
        for (auto &i : com.Recv()) {
            ServerQue[index].RecvingQue.push_back(i);
        }
        
        for (auto &i : ServerQue[index].SendingQue) {
            com.Send(i);
        }

        ServerQue[index].SendingQue.clear();

        if (ServerQue[index].shutdown) {
            kill = true;
            break;
        }
        static int iter_connected = 0;
        iter_connected++;

        if (iter_connected > 4 && HtmlHost) {
            break;
        }

    }

   
#ifdef _WIN32
// Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();
#endif // DEBUG

#ifdef __linux
    close(clientSocket);
#endif // DEBUG
    

    dis.out(D_WARNING, "Server socket closing...");

    SocketIds[LookUpArrayId(id)] = -1;
    ServerActivity = true;


}


