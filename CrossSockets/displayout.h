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

// To Disable COLOR, INFO, WARNING, ERROR, DEBUG, and or LOG use 1 to enable use 0
#define D_DISABLE_COLOR       0
#define D_DISABLE_INFO        0
#define D_DISABLE_WARNING     0
#define D_DISABLE_ERROR       0
#define D_DISABLE_DEBUG       0
#define D_DISABLE_LOG         0

// define this if you want to backup control
#define BACKUP_CON

// defines colors if they are not defined.
#ifdef BACKUP_CON   
#ifndef D_DISABLE_COLOR
#define D_DISABLE_COLOR 0 
#endif 
#ifndef D_DISABLE_INFO
#define D_DISABLE_INFO 0
#endif 
#ifndef D_DISABLE_WARNING
#define D_DISABLE_WARNING 0
#endif 
#ifndef D_DISABLE_ERROR
#define D_DISABLE_ERROR 0
#endif 
#ifndef D_DISABLE_DEBUG
#define D_DISABLE_DEBUG 0
#endif 
#ifndef D_DISABLE_LOG
#define D_DISABLE_LOG
#endif
#endif

std::atomic<bool> __IsDisplaying = false;


class displayout
{
public:

    displayout();
    ~displayout();

    // Check if we have color enabled
#if D_DISABLE_COLOR == 0
    void out(int enumTypeColor, std::string ToDisplay);
#else
    void out(std::string ToDisplay);
#endif
    
protected:
    

private:
    std::atomic<std::vector<std::string>> PreDisplayControl;

    void Threaded_Display();
};

