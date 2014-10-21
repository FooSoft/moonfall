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
class RefCount
{
public:
    RefCount(int history = 100);

    boost::shared_ptr<T>    FindClient(Token id) const;
    void                    AddClient(Token id, const boost::shared_ptr<T>& client);
    void                    RemoveClient(Token id);
    void                    ClearClients();

private:
    void                    CacheClient(const boost::shared_ptr<T>& client);

    typedef                 std::map<Token, boost::weak_ptr<T>, std::less<int> >    ClientMap;
    typedef                 std::list<boost::shared_ptr<T> >                        ClientList;

    ClientList              m_list;
    ClientMap               m_map;
    int                     m_history;
};

#include "RefCount.inl"
