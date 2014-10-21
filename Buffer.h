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

class Buffer
{
public:
    Buffer(const void* data, int size);
    Buffer(const Buffer& other);
    explicit Buffer(int size = 0);
    ~Buffer();

    void        AddToBack(const void* data, int size);
    void        AddToFront(const void* data, int size);
    int         RemoveFromFront(void* data, int size);
    int         RemoveFromBack(void* data, int size);
    void*       Allocate(int size);
    void        Clear();

    void        Set(const void* data, int size);
    const void* Get() const;
    void*       Get();
    int         GetSize() const;

    Buffer&     operator =(const Buffer& other);
    const byte& operator [](int index) const;
    byte&       operator [](int index);

private:
    byte*       m_data;
    int         m_maxSize;
    int         m_size;
};
