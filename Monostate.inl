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
Monostate<T>::Monostate() :
        m_member(NULL)
{
}

template <typename T>
Monostate<T>::~Monostate()
{
    Reset();
}

template <typename T>
T* Monostate<T>::operator ->()
{
    return GetMemberSafe();
}

template <typename T>
const T* Monostate<T>::operator ->() const
{
    return GetMemberSafe();
}

template <typename T>
T& Monostate<T>::operator *()
{
    return *GetMemberSafe();
}

template <typename T>
const T& Monostate<T>::operator *() const
{
    return *GetMemberSafe();
}

template <typename T>
Monostate<T>::operator T* ()
{
    return GetMemberSafe();
}

template <typename T>
Monostate<T>::operator const T* () const
{
    return GetMemberSafe();
}

template <typename T>
void Monostate<T>::Reset()
{
    if (m_member != NULL)
    {
        delete m_member;
        m_member = NULL;
    }
}

template <typename T>
const T* Monostate<T>::GetMemberSafe() const
{
    if (m_member == NULL)
    {
        m_member = new T();
    }

    return m_member;
}

template <typename T>
T* Monostate<T>::GetMemberSafe()
{
    if (m_member == NULL)
    {
        m_member = new T();
    }

    return m_member;
}
