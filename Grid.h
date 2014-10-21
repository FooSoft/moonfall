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

template <typename T, typename V>
class Grid
{
public:
    Grid(int chunkDims = 256);

    bool            Add(const T& data, const V& id, const Vector2i& position);
    bool            Remove(const V& id);
    bool            Update(const V& id, const Vector2i& position);
    bool            Contains(const V& id) const;
    const T*        Get(const V& id) const;
    T*              Get(const V& id);
    void            Query(std::vector<T>* data, const Rect4i& bounds) const;
    void            Enumerate(std::vector<T>* data) const;
    void            Clear();

private:
    struct          Entry;
    typedef         qword                                   ChunkKey;
    typedef         dword                                   ChunkKeyCoord;
    typedef         std::map<V, boost::shared_ptr<Entry> >  DataMap;
    typedef         std::map<ChunkKey, DataMap>             ChunkMap;

    ChunkKey        PositionToKey(const Vector2i& position) const;

    struct Entry
    {
        Entry (const T& data, const Vector2i& position) :
                data(data), position(position) { }

        T           data;
        Vector2i    position;
    };

    ChunkMap        m_chunks;
    DataMap         m_data;
    int             m_chunkDims;
};

#include "Grid.inl"
