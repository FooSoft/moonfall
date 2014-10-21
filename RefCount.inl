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

template <typename T>
RefCount<T>::RefCount(int history) :
        m_history(history)
{
}

template <typename T>
boost::shared_ptr<T> RefCount<T>::FindClient(Token id) const
{
    typename ClientMap::const_iterator iter = m_map.find(id);
    return iter == m_map.end() ? boost::shared_ptr<T>() : iter->second.lock();
}

template <typename T>
void RefCount<T>::AddClient(Token id, const boost::shared_ptr<T>& client)
{
    if (FindClient(id))
    {
        m_map[id] = client;
    }
    else
    {
        m_map.insert(std::make_pair(id, boost::weak_ptr<T>(client)));
    }

    CacheClient(client);
}

template <typename T>
void RefCount<T>::RemoveClient(Token id)
{
    typename ClientMap::iterator iter = m_map.find(id);
    if (iter != m_map.end())
    {
        m_map.erase(iter);
    }
}

template <typename T>
void RefCount<T>::ClearClients()
{
    m_map.clear();
    m_list.clear();
}

template <typename T>
void RefCount<T>::CacheClient(const boost::shared_ptr<T>& client)
{
    if (m_history == 0)
    {
        return;
    }

    typename ClientList::iterator iter = find(m_list.begin(), m_list.end(), client);
    if (iter == m_list.end())
    {
        m_list.push_back(client);
        if (m_list.size() > static_cast<unsigned>(m_history))
        {
            m_list.erase(m_list.begin());
        }
    }
    else
    {
        m_list.erase(iter);
        m_list.push_back(client);
    }
}
