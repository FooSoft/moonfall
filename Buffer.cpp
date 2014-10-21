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
#include "Buffer.h"

Buffer::Buffer(const void* data, int size) :
        m_data(NULL),
        m_maxSize(0),
        m_size(0)
{
    AddToBack(data, size);
}

Buffer::Buffer(const Buffer& other) :
        m_data(NULL),
        m_maxSize(0),
        m_size(0)
{
    *this = other;
}

Buffer::Buffer(int size) :
        m_data(NULL),
        m_maxSize(0),
        m_size(0)
{
    Allocate(size);
}

Buffer::~Buffer()
{
    if (m_data != NULL)
    {
        delete[] m_data;
    }
}

void Buffer::AddToBack(const void* data, int size)
{
    if (size <= 0)
    {
        return;
    }

    if (m_size + size > m_maxSize)
    {
        m_maxSize = (m_size + size) * 2;
        byte* temp = new byte[m_maxSize + 1];
        if (m_data != NULL)
        {
            memcpy(temp, m_data, m_size);
            delete[] m_data;
        }
        m_data = temp;
    }

    memcpy(m_data + m_size, data, size);
    m_size += size;

    m_data[m_size] = 0;
}

void Buffer::AddToFront(const void* data, int size)
{
    if (size <= 0)
    {
        return;
    }

    if (m_size + size > m_maxSize)
    {
        m_maxSize = (m_size + size) * 2;
    }

    byte* temp = new byte[m_maxSize + 1];
    memcpy(temp, data, size);
    if (m_data != NULL)
    {
        memcpy(temp + size, m_data, m_size);
        delete[] m_data;
    }
    m_data = temp;
    m_size += size;

    m_data[m_size] = 0;
}

int Buffer::RemoveFromFront(void* data, int size)
{
    size = MIN(size, m_size);
    if (size <= 0)
    {
        return 0;
    }

    m_size -= size;

    if (data != NULL)
    {
        memcpy(data, m_data, size);
    }

    memmove(m_data, m_data + size, m_size);

    if (m_size > 0)
    {
        m_data[m_size] = 0;
    }

    return size;
}

int Buffer::RemoveFromBack(void* data, int size)
{
    size = MIN(size, m_size);
    if (size <= 0)
    {
        return 0;
    }

    m_size -= size;

    if (data != NULL)
    {
        memcpy(data, m_data + m_size, size);
    }

    if (m_size > 0)
    {
        m_data[m_size] = 0;
    }

    return size;
}

int Buffer::GetSize() const
{
    return m_size;
}

void Buffer::Set(const void* data, int size)
{
    if (size > m_maxSize)
    {
        if (m_data != NULL)
        {
            delete[] m_data;
        }

        m_maxSize = size * 2;
        m_data = new byte[m_maxSize + 1];
    }

    memcpy(m_data, data, size);
    m_size = size;

    if (m_size > 0)
    {
        m_data[m_size] = 0;
    }
}

const void* Buffer::Get() const
{
    return m_size > 0 ? m_data : NULL;
}

void* Buffer::Get()
{
    return m_size > 0 ? m_data : NULL;
}

void* Buffer::Allocate(int size)
{
    if (size > m_maxSize)
    {
        if (m_data != NULL)
        {
            delete[] m_data;
        }

        m_maxSize = size * 2;
        m_data = new byte[m_maxSize + 1];
    }

    m_size = size;

    if (m_size > 0)
    {
        m_data[m_size] = 0;
    }

    return m_data;
}

void Buffer::Clear()
{
    m_size = 0;
}

Buffer& Buffer::operator =(const Buffer& other)
{
    if (this != &other)
    {
        Set(other.Get(), other.GetSize());
    }

    return *this;
}

const byte& Buffer::operator [](int index) const
{
    ASSERT(index < m_size);
    return m_data[index];
}

byte& Buffer::operator [](int index)
{
    ASSERT(index < m_size);
    return m_data[index];
}
