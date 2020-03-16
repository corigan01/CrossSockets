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

#ifndef BASE__INCLUDE__H__
#define BASE__INCLUDE__H__ 

//#inlcude 
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <ostream>
#include <atomic>
#include <time.h>  
#include <cstdio>
#include <sstream>
#include <algorithm>


#ifdef _WIN32
#include <WS2tcpip.h>
#include <chrono>

#pragma comment (lib, "ws2_32.lib")

#endif // _WIN32

#ifdef __linux

#endif // __linux



#define EOR std::cout << "[DEBUGING USE ONLY] PASSED LINE: " << __LINE__ << std::endl;

#ifndef _STD
#define _STD ::std::
#endif // !_STD



// displays out the log
/*void displayout(int msgType, const char* text, ...)
{
	std::cout << debugcolor[msgType];

	va_list args;
	va_start(args, text);
	fprintf(stdout, (debugstring[msgType] + ": ").c_str());
	vfprintf(stdout, text, args);
	va_end(args);


	fprintf(stdout, "\n");
	std::cout << debugcolor[0];

}*/

// Defining Helpful macros 
#define FOR(varname, iterations) for (int varname = 0; varname < iterations; varname++)
#define xstr(x) str(x)
#define str(x) #x
#define glue(x, y) x ## y




#endif // !Template