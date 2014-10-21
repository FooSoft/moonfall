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

template <typename T, typename V>
Grid<T, V>::Grid(int chunkDims) :
        m_chunkDims(chunkDims)
{
}

template <typename T, typename V>
bool Grid<T, V>::Add(const T& data, const V& id, const Vector2i& position)
{
    if (Contains(id))
    {
        return false;
    }

    const ChunkKey key = PositionToKey(position);
    typename ChunkMap::iterator chunkIter = m_chunks.find(key);
    if (chunkIter == m_chunks.end())
    {
        chunkIter = m_chunks.insert(std::make_pair(key, DataMap())).first;
    }

    const boost::shared_ptr<Entry> entry(new Entry(data, position));
    DataMap& dataMap = chunkIter->second;
    dataMap.insert(std::make_pair(id, entry));
    m_data.insert(std::make_pair(id, entry));

    return true;
}

template <typename T, typename V>
bool Grid<T, V>::Remove(const V& id)
{
    if (!Contains(id))
    {
        return false;
    }

    const typename DataMap::iterator dataIter = m_data.find(id);
    const ChunkKey key = PositionToKey(dataIter->second->position);
    m_data.erase(dataIter);

    const typename ChunkMap::iterator chunkIter = m_chunks.find(key);
    DataMap& dataMap = chunkIter->second;
    dataMap.erase(id);
    if (dataMap.empty())
    {
        m_chunks.erase(chunkIter);
    }

    return true;
}

template <typename T, typename V>
bool Grid<T, V>::Update(const V& id, const Vector2i& position)
{
    if (!Contains(id))
    {
        return false;
    }

    const typename DataMap::iterator dataIter = m_data.find(id);
    const boost::shared_ptr<Entry> entry = dataIter->second;
    const ChunkKey oldKey = PositionToKey(entry->position);
    const ChunkKey newKey = PositionToKey(position);

    if (oldKey == newKey)
    {
        entry->position = position;
    }
    else
    {
        Remove(id);
        Add(entry->data, id, position);
    }

    return true;
}

template <typename T, typename V>
void Grid<T, V>::Query(std::vector<T>* data, const Rect4i& bounds) const
{
    const Vector2i begin = Vector2i(bounds.x0, bounds.y0) / m_chunkDims;
    const Vector2i end = Vector2i(bounds.x1, bounds.y1) / m_chunkDims;

    Vector2i position;

    for (int y = begin.y; y <= end.y; ++y)
    {
        position.y = y * m_chunkDims;

        for (int x = begin.x; x <= end.x; ++x)
        {
            position.x = x * m_chunkDims;

            const typename ChunkMap::const_iterator chunkIter = m_chunks.find(PositionToKey(position));
            if (chunkIter == m_chunks.end())
            {
                continue;
            }

            for (typename DataMap::const_iterator dataIter = chunkIter->second.begin(); dataIter != chunkIter->second.end(); ++dataIter)
            {
                const boost::shared_ptr<Entry> entry = dataIter->second;
                if (bounds.Contains(entry->position.x, entry->position.y))
                {
                    data->push_back(entry->data);
                }
            }
        }
    }
}

template <typename T, typename V>
void Grid<T, V>::Enumerate(std::vector<T>* data) const
{
    for (typename DataMap::const_iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
    {
        data->push_back(iter->second->data);
    }
}

template <typename T, typename V>
bool Grid<T, V>::Contains(const V& id) const
{
    return m_data.find(id) != m_data.end();
}

template <typename T, typename V>
const T* Grid<T, V>::Get(const V& id) const
{
    const typename DataMap::const_iterator iter = m_data.find(id);
    return iter == m_data.end() ? NULL : &iter->second->data;
}

template <typename T, typename V>
T* Grid<T, V>::Get(const V& id)
{
    const typename DataMap::iterator iter = m_data.find(id);
    return iter == m_data.end() ? NULL : &iter->second->data;
}

template <typename T, typename V>
void Grid<T, V>::Clear()
{
    m_data.clear();
    m_chunks.clear();
}

template <typename T, typename V>
typename Grid<T, V>::ChunkKey Grid<T, V>::PositionToKey(const Vector2i& position) const
{
    union
    {
        struct
        {
            ChunkKeyCoord   x;
            ChunkKeyCoord   y;
        }                   position;
        ChunkKey            key;
    } map;

    map.position.x = position.x / m_chunkDims;
    map.position.y = position.y / m_chunkDims;

    return map.key;
}
