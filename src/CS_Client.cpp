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
#include "CS_Client.h"

CS_Client::CS_Client(int port, std::string Ip, int TryConnectionCount, bool AddStringBuildBack) {
    Port = port;
    IP = Ip;
    BuildBack = AddStringBuildBack;
    TryCount = TryConnectionCount;

    StartClient();
}

CS_Client::~CS_Client() {
    StopClient();
}


void CS_Client::StopClient() {
    while (true) {
        sleep_for(66ms);
        if (ClientsThread.joinable()) {
            ClientsThread.join();
            break;
        }
    }
    dis.out(D_WARNING, "Stopped Threads");
}

void CS_Client::StartClient() {
    dis.out(D_INFO, "Started Threads");

    ClientsThread = std::thread(&CS_Client::ClientThread, this);

}

std::vector<std::string> CS_Client::GetRec() {
    return Recv;
}

void CS_Client::Send(std::string data) {
    Sendv.push_back(data);
}


void CS_Client::ClientThread() {
/*__START:

    Sleep(100);
    dis.out(D_INFO, "Client STARTING ...");
    dis.out(D_INFO, "Client Starting Connection: %s::%d");

    dis.out(D_INFO, "Client Windows Platform - Using WinSock");

    WSAData Data;
    WORD ver = MAKEWORD(2, 2);

    int WS_START = WSAStartup(ver, &Data);
    if (WS_START != 0) {
        dis.out(D_ERROR, "Client Unable To Start WinSock. ERROR #%d");

        return;
    }

    // Create The Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        dis.out(D_ERROR, "Client Can't create socket, Err #%s");
        WSACleanup();
        return;
    }

    dis.out(D_INFO, "Client Winsock Started!");
    dis.out(D_INFO, "Client Starting hint structure...");

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(Port);
    inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

    // Connect to server

    dis.out(D_INFO, "Client Setup complete!");

    int TryCounting = 0;
    do {
        sleep_for(33ms);
        dis.out(D_INFO, "Client Conneting...");
        int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connResult == SOCKET_ERROR)
        {
            dis.out(D_WARNING, "Client Connection Failed, Retrying...");
            TryCounting++;
        }
        else {
            break;
        }
        if (TryCounting >= TryCount) {
            StopAll = true;
            dis.out(D_ERROR, "Client Connection Failed! Server Not Found! Stopping Threads...");
            return;
        }
    } while (!StopAll);

    
    CS_COM com(&sock, false);
    dis.out(D_INFO, "Client CONNECTED");
    
    while (!StopAll) {
        sleep_for(33ms);
        

        for (auto i : com.Recv()) {
            Recv.push_back(i);
        }

        for (auto i : Sendv) {
            com.Send(i);
        }

        Sendv.clear();

        if (com.Error()) {
            dis.out(D_ERROR, "Error, restarting Client...");
            goto __START;
        }

    }

*/
}