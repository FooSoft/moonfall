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

#include "Pch.h"
#include "Serialization.h"

Deserializer::Deserializer(const Buffer* data) :
        m_data(data),
        m_offset(0)
{
}

const char* Deserializer::ReadString()
{
    const char* start = static_cast<const char*>(m_data->Get()) + m_offset;
    const char* current = start;

    while (m_offset < m_data->GetSize())
    {
        ++m_offset;
        if (*current == 0)
        {
            return start;
        }
        ++current;
    }

    return NULL;
}

const byte* Deserializer::Read(int size)
{
    if (size > m_data->GetSize() - m_offset)
    {
        return NULL;
    }

    const byte* data = static_cast<const byte*>(m_data->Get()) + m_offset;
    m_offset += size;

    return data;
}

template <>
bool Deserializer::Read<std::string>(std::string* result)
{
    const char* data = ReadString();
    if (data == NULL)
    {
        return false;
    }
    *result = data;
    return true;
}

bool Deserializer::ReadString(std::string* result)
{
    return Read(result);
}

bool Deserializer::ReadString(char* result, int count)
{
    const char* data = ReadString();
    if (data == NULL)
    {
        return false;
    }
    strncpy(result, data, count - 1);
    return true;
}

int Deserializer::GetOffset() const
{
    return m_offset;
}

bool Deserializer::SetOffset(int offset, bool relative)
{
    const int offsetNew = relative ? m_offset + offset : offset;

    if (offsetNew >= 0 && offsetNew <= m_data->GetSize())
    {
        m_offset = offsetNew;
        return true;
    }

    return false;
}

Serializer::Serializer(Buffer* data) :
        m_data(data),
        m_offset(0)
{
}

void Serializer::WriteString(const std::string& data)
{
    WriteString(data.c_str());
}

void Serializer::WriteString(const char* data)
{
    Write(data, strlen(data) + 1);
}

void Serializer::Write(const void* data, int size)
{
    m_data->AddToBack(static_cast<const byte*>(data), size);
    m_offset += size;
}

bool Serializer::SetOffset(int offset, bool relative)
{
    const int offsetNew = relative ? m_offset + offset : offset;

    if (offsetNew >= 0 && offsetNew <= m_data->GetSize())
    {
        m_offset = offsetNew;
        return true;
    }

    return false;
}

int Serializer::GetOffset() const
{
    return m_offset;
}
