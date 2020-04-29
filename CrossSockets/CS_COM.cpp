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
#include "CS_COM.h"

CS_COM::CS_COM(SOCKET *sock, bool NoOutput) {
    SendingThr = std::thread(&CS_COM::SendThread, this);
    RecvingThr = std::thread(&CS_COM::RecvThread, this);
    this->sock = sock;

    NoOut = NoOutput;

    if (!NoOut) Displ.out(D_INFO, "Stack Size of block is " + std::to_string(IO_SIZE) + " Bytes");
}

CS_COM::~CS_COM() {
    KillThreads = true;

    

    SendingThr.join();
    RecvingThr.join();
    

}

void CS_COM::Send(std::string Send) {
    std::vector<std::string> BrokenUp;

    //BrokenUp.push_back("\h");

    std::string Built;
    for (int i = 0; i < Send.size(); i++) {
        if (Built.length() < IO_SIZE) {
            Built += Send[i];
        }
        else {
            BrokenUp.push_back(Built);
            Built = Send[i];
        }
    }
    BrokenUp.push_back(Built); //+"\g");
    
    
    


    for (auto i : BrokenUp) {
        if(!NoOut) Displ.out(D_INFO, i);
        //if(!NoOut) Displ.out(D_INFO, std::to_string(Built.length()) + " Bytes");

        SendingQue.push_back(i);
    }

}

std::vector<std::string> CS_COM::Recv() {


    if (RecvingQue.size() > 0) {
        auto Sendout = RecvingQue;
        RecvingQue.clear();
        
        return Sendout;
    }

    std::vector<std::string> non;
    return non;

}

void CS_COM::SendThread() {
    while (!KillThreads) {
        sleep_for(16ms);
        for (int i = 0; i < SendingQue.size(); i++) {
            send((SOCKET)*sock, SendingQue[i].c_str(), SendingQue[i].length() + 1, 0);
        }
        
        SendingQue.clear();
        
    }


}

void CS_COM::RecvThread() {
   
    while (!KillThreads) {
        sleep_for(16ms);
        char buf[IO_SIZE + 20];
        ZeroMemory(buf, IO_SIZE + 20);

        // Wait for client to send data
        int bytesReceived = recv((SOCKET)*sock, buf, IO_SIZE + 20, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            if(!NoOut) Displ.out(D_ERROR, "Error in recv, Quitting Thread...");
            ComError = true;
            break;
        }
        if (bytesReceived == 0)
        {
            if(!NoOut) Displ.out(D_WARNING, "Disconnected...");
            ComError = true;
            break;
        }
       
        std::string str = std::string(buf);

        if (str.size() == 0)
            continue;

        /*else if (str[0] == '\h' && str.back() == '\g') {
            std::string Mstr = str;

            Mstr.erase(Mstr.begin());
            Mstr.erase(Mstr.end());


            RecvingQue.push_back(Mstr);
        }

        else if (str[0] == '\n' && str.back() != '\g') {
            std::string Mstr = str;

            Mstr.erase(Mstr.begin());

            HoldingPos += Mstr;
        }

        else if (str[0] != '\n' && str.back() == '\g') {
            std::string Mstr = str;

            Mstr.erase(Mstr.end());

            RecvingQue.push_back(HoldingPos + Mstr);

            HoldingPos = "";

        }

        else {
            HoldingPos += str;
        }*/

        RecvingQue.push_back(str);

        if(!NoOut) Displ.out(D_INFO, str);


    }

    KillThreads = true;
}