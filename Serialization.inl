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

template <typename T>
bool Deserializer::Read(T* result)
{
    const T* data = reinterpret_cast<const T*>(Read(sizeof(T)));
    if (data == NULL)
    {
        return false;
    }
    *result = *data;
    return true;
}

template <typename T>
bool Deserializer::ReadArray(T* result, int count)
{
    const int size = sizeof(T) * count;
    const T* data = reinterpret_cast<const T*>(Read(size));
    if (data == NULL)
    {
        return false;
    }
    memcpy(result, data, size);
    return true;
}

template <typename T>
void Serializer::Write(const T& data)
{
    Write(&data, sizeof(T));
}

template <typename T>
void Serializer::WriteArray(const T* data, int count)
{
    Write(data, sizeof(T) * count);
}
