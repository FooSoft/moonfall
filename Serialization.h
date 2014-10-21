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

class Deserializer
{
public:
    Deserializer(const Buffer* data);

    template <typename T>
    bool            Read(T* result);
    template <typename T>
    bool            ReadArray(T* result, int count);
    bool            ReadString(std::string* result);
    bool            ReadString(char* result, int count);
    const byte*     Read(int size);
    bool            SetOffset(int offset, bool relative);
    int             GetOffset() const;

private:
    const char*     ReadString();

    const Buffer*   m_data;
    int             m_offset;
};

class Serializer
{
public:
    Serializer(Buffer* data);

    template <typename T>
    void            Write(const T& data);
    template <typename T>
    void            WriteArray(const T* data, int count);
    void            WriteString(const std::string& data);
    void            WriteString(const char* data);
    void            Write(const void* data, int size);
    bool            SetOffset(int offset, bool relative);
    int             GetOffset() const;

private:
    Buffer*         m_data;
    int             m_offset;
};

#include "Serialization.inl"
