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




class CS_COM
{
public:
    CS_COM(SOCKET Socket);
    ~CS_COM(/*Global*/);

protected:
    void Send();
    


};

/*
    char buf[bufferSize + 10];
    ZeroMemory(buf, bufferSize + 10);

    // Wait for client to send data
    int bytesReceived = recv(sock, buf, bufferSize + 10, 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        out(D_ERROR, "Error in recv, Quitting Thread...");
        ThreadShouldStop = true;
    }

    if (bytesReceived == 0)
    {
        out(D_WARNING, "Disconnected...");
        ThreadShouldStop = true;
    }
    else {
        //out(D_DEBUG, std::to_string(bytesReceived).c_str());
    }
    ThreadShouldRestart = true;
    return std::string(buf);
}

void CPSocket::TxV(SOCKET COM, std::string Data) {
    send(COM, Data.c_str(), Data.length() + 1, 0);
}
*/