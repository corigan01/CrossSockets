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

#include "displayout.h"


displayout::displayout() {
    // Init the display thread

    ThreadDisplay = std::thread(&displayout::Threaded_Display, this);


}

displayout::~displayout() {
    // Join the main thread from here
    SetToQuit = true;

    // if it takes too long for the thread to join then we can hang here
    /// TODO fix this hang
    while (ThreadDisplay.joinable()) {
        ThreadDisplay.join();
    }
 
}

void displayout::out(int enumTypeColor, std::string ToDisplay) {
    auto ThreadPart = PreDisplayControl;

    D_COLOR::ColorM::Modifier ModColor[] = { D_COLOR::defM, D_COLOR::blueM, D_COLOR::greenM, D_COLOR::redM, D_COLOR::redM, D_COLOR::magentaM, D_COLOR::yellowM };

    D_COLOR::__OBJSTRING ColorBack = { ToDisplay, D_COLOR::debugstring[enumTypeColor], ModColor[enumTypeColor]};

    ThreadPart.push_back(ColorBack);

    PreDisplayControl = ThreadPart;
}

void displayout::Threaded_Display() {
    // Start the init for the display thread that will be displaying all content from a thread(s)
    while (!SetToQuit) {
        sleep_for(66ms);

        if (!__IsDisplaying) {
        auto ThreadPart = PreDisplayControl;
        PreDisplayControl.clear();

            if (ThreadPart.size() > 0) {
                __IsDisplaying = true;

                for (auto i : ThreadPart) {
                    if (i.Message.size() > 0) {
                        std::cout << i.DebugType << "[" << i.Prompt << "]: " << i.Message << D_COLOR::defM << std::endl;
                        AllLog.push_back(i.Message);
                    }
                }
                __IsDisplaying = false;
            }
        }
    }

}