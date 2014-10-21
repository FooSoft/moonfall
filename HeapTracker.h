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

namespace Heap
{
    struct Block
    {
        char        filename[PATH_MAX];
        const void* address;
        size_t      size;
        int         line;
    };

    void TrackBegin();
    void TrackEnd();
    bool HasAllocs();
    void DumpAllocs(std::vector<Block>* blocks);
}

void*   operator new(size_t size, const char* filename, int line);
void*   operator new[](size_t size, const char* filename, int line);
void    operator delete(void* ptr, const char* filename, int line);
void    operator delete[](void* ptr, const char* filename, int line);

#define new new(__FILE__, __LINE__)
