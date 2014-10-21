/*
    Moonfall  Copyright (C) 2008  Alex Yatskov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef WINDOWS
    #define _CRT_SECURE_NO_WARNINGS

    #pragma comment(lib, "SDL_image.lib")
    #pragma comment(lib, "SDL_ttf.lib")
    #pragma comment(lib, "SDL.lib")
    #pragma comment(lib, "lua5.1.lib")
    #pragma comment(lib, "zdll.lib")
    #pragma comment(lib, "comctl32.lib")
    #pragma comment(lib, "rpcrt4.lib")

    #ifdef DEBUG
        #pragma comment(lib, "wxbase28ud.lib")
        #pragma comment(lib, "wxmsw28ud_core.lib")
        #pragma comment(lib, "wxmsw28ud_adv.lib")
        #pragma comment(lib, "wxpngd.lib")
        #pragma comment(lib, "wxjpegd.lib")
        #pragma comment(lib, "wxtiffd.lib")
    #else
        #pragma comment(lib, "wxbase28u.lib")
        #pragma comment(lib, "wxmsw28u_core.lib")
        #pragma comment(lib, "wxmsw28u_adv.lib")
        #pragma comment(lib, "wxpng.lib")
        #pragma comment(lib, "wxjpeg.lib")
        #pragma comment(lib, "wxtiff.lib")
    #endif
#endif
