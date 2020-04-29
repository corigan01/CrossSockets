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
#include "displayout.h"  

struct ServVect {
    std::string Name;
    std::vector<std::string> RecvingQue;
    std::vector<std::string> SendingQue;
    bool shutdown = false;
};



class CS_Server
{
public:
    CS_Server(int port, bool AddBuildBack = false, bool htmlHost = false);
    ~CS_Server();

    std::vector<std::string> GetAllClientNames();
    std::vector<ServVect>*   GetAllClientSerVect();
    ServVect* GetClientSerVect(std::string Name);
    void HostHtml(std::string Html);

    void SendToClient(std::string Name, std::string Send);
    void SendToAll(std::string Send);

    void DisconnectClient(std::string Name);
    void SetConnections(bool connect);
    void DisconnectAll();
    

    int LookUpArrayId(int id) {
        for (int i = 0; i < SocketIds.size(); i++) {
            if (SocketIds[i] == id) {
                return i;
            }
        }
        return -1;
    }


private:
    std::vector<std::thread>   ThreadPool       ;
    std::thread                SMT;             // Server Manager Thread
    std::vector<std::string>   ClientNames      ;
    std::vector<int>           SocketIds        ;
    std::vector<ServVect>      ServerQue        ;
    int                        ConnectionCount  = 0;
    bool                       ServerActivity   = false;
    bool                       ServersFull      = true;
    bool                       StopAll          = false;
    bool                       stringBuildback  = false;
    bool                       HtmlHost         = false;
    bool                       Disconnect       = false;
    Server                     ConnectionProp   ;
    displayout                 dis              ;
    std::string                HtmlF            ;
    


    void ServerManager ();
    void ServerThread  (int id);
};

