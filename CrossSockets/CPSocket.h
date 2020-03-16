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

#ifndef Template
#define Template 

#include "Base.h" 
#include "displayout.h"

struct Server
{
    std::string IP = "127.0.0.1";
    int InBoundPort = 56010;
    int OutBound = 56050;
};

class CPSocket : private displayout
{
public:
    CPSocket();
    CPSocket(std::string IP, int TxPort, int RxPort);
    ~CPSocket();

    void StartClient();
    void StartServer();

    bool IsConnected();
    bool IsAuth();

    void SendString(std::string RecStr);
    std::vector<std::string> GetString();
    
    


private:
    std::thread SocketThreadRx;
    std::thread SocketThreadTx;
    std::thread ServerManagerThread;

    std::vector<std::thread> ServerThreadsRx;
    std::vector<std::thread> ServerThreadsTx;
    std::vector<std::string> HostNames;
    std::vector<int> AliveServers;
    int ServerConnectionCount = 0;

    bool ServerActivity = false;
    bool ServersFull = true;

    Server ConnectionProp;

    std::vector<std::string> RxQue;
    std::vector<std::string> TxQue;

    bool IsConnectedTx = false;
    bool IsConnectedRx = false;
    bool IsAuthSSC = false;
    bool IsSocketsFull = false;
    bool ThreadShouldStop = false;
    bool ThreadShouldRestart = false;

    int AUTH_NUMBER = 0;

    void ClientTx();
    void ClientRx();


    void ServerManager();
    void ServerTx(int);
    void ServerRx(int);

    

    int LookUpArrayId(int id) {
        for (int i = 0; i < AliveServers.size(); i++) {
            if (AliveServers[i] == id) {
                return i;
            }
        }
        return -1;
    }

    void TxV(SOCKET COM, std::string Data);

    std::string RxV(SOCKET);
};


#endif // !Template







