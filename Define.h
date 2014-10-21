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

#pragma once

#define BIT(x)                  (1 << (x))
#define MIN(x, y)               ((x) < (y) ? (x) : (y))
#define MAX(x, y)               ((x) > (y) ? (x) : (y))
#define ARRAY_SIZE(x)           static_cast<int>(sizeof(x) / sizeof(x[0]))
#define ZERO(x)                 memset(&x, 0, sizeof(x))
#define ZERO_PTR(x)             memset(x, 0, sizeof(0))
#define IS_TRUE(x)              ((x) ? true : false)
#define IS_FALSE(x)             (!IS_TRUE(x))

#ifdef WINDOWS
    #define                     PATH_MAX MAX_PATH
#endif

#ifdef WINDOWS
    typedef unsigned __int8     byte;
    typedef unsigned __int16    word;
    typedef unsigned __int32    dword;
    typedef unsigned __int64    qword;
#else
    typedef unsigned char       byte;
    typedef unsigned short      word;
    typedef unsigned int        dword;
    typedef unsigned long int   qword;
#endif

BOOST_STATIC_ASSERT(sizeof(byte) == 1);
BOOST_STATIC_ASSERT(sizeof(word) == 2);
BOOST_STATIC_ASSERT(sizeof(dword) == 4);
BOOST_STATIC_ASSERT(sizeof(qword) == 8);
