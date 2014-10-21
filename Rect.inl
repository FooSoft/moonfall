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
Rect4<T>::Rect4(T x0, T y0, T x1, T y1) :
        x0(x0),
        y0(y0),
        x1(x1),
        y1(y1)
{
}

template <typename T>
Rect4<T>::Rect4(const Rect4& rect) :
        x0(0),
        y0(0),
        x1(0),
        y1(0)
{
    *this = rect;
}

template <typename T>
Rect4<T>::Rect4(T v) :
        x0(v),
        y0(v),
        x1(v),
        y1(v)
{
}

template <typename T>
Rect4<T>::Rect4() :
        x0(0),
        y0(0),
        x1(0),
        y1(0)
{
}

template <typename T>
bool Rect4<T>::Intersects(const Rect4& rect) const
{
    return !(y1 <= rect.y0 || y0 >= rect.y1 || x1 <= rect.x0 || x0 >= rect.x1);
}

template <typename T>
Rect4<T> Rect4<T>::Intersection(const Rect4& rect) const
{
    return Rect4<T>(
               MAX(x0, rect.x0),
               MAX(y0, rect.y0),
               MIN(x1, rect.x1),
               MIN(y1, rect.y1)
           );
}

template <typename T>
bool Rect4<T>::Contains(const Rect4& rect) const
{
    return Contains(rect.x0, rect.y0) && Contains(rect.x1, rect.y1);
}

template <typename T>
bool Rect4<T>::Contains(T x, T y) const
{
    return x >= x0 && y >= y0 && x < x1 && y < y1;
}

template <typename T>
T Rect4<T>::GetWidth() const
{
    return x1 - x0;
}

template <typename T>
T Rect4<T>::GetHeight() const
{
    return y1 - y0;
}

template <typename T>
bool Rect4<T>::operator ==(const Rect4& rect) const
{
    return x0 == rect.x0 && y0 == rect.y0 && x1 == rect.x1 && y1 == rect.y1;
}

template <typename T>
bool Rect4<T>::operator !=(const Rect4& rect) const
{
    return !(*this == rect);
}

template <typename T>
Rect4<T>& Rect4<T>::operator =(const Rect4& rect)
{
    if (this != &rect)
    {
        x0 = rect.x0;
        y0 = rect.y0;
        x1 = rect.x1;
        y1 = rect.y1;
    }

    return *this;
}

template <typename T>
Rect4<T> Rect4<T>::operator +(const Rect4& rect) const
{
    return Rect4<T>(
               x0 + rect.x0,
               y0 + rect.y0,
               x1 + rect.x1,
               y1 + rect.y1
           );
}

template <typename T>
Rect4<T> Rect4<T>::operator -(const Rect4& rect) const
{
    return Rect4<T>(
               x0 - rect.x0,
               y0 - rect.y0,
               x1 - rect.x1,
               y1 - rect.y1
           );
}

template <typename T>
Rect4<T> Rect4<T>::operator *(T scalar) const
{
    return Rect4<T>(
               x0 * scalar,
               y0 * scalar,
               x1 * scalar,
               y1 * scalar
           );
}

template <typename T>
Rect4<T> Rect4<T>::operator /(T scalar) const
{
    return Rect4<T>(
               x0 / scalar,
               y0 / scalar,
               x1 / scalar,
               y1 / scalar
           );
}

template <typename T>
Rect4<T>& Rect4<T>::operator +=(const Rect4& rect)
{
    *this = *this + rect;
    return *this;
}

template <typename T>
Rect4<T>& Rect4<T>::operator -=(const Rect4& rect)
{
    *this = *this - rect;
    return *this;
}

template <typename T>
Rect4<T>& Rect4<T>::operator *=(T scalar)
{
    *this = *this * scalar;
    return *this;
}

template <typename T>
Rect4<T>& Rect4<T>::operator /=(T scalar)
{
    *this = *this / scalar;
    return *this;
}
