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
class Rect4
{
public:
    Rect4(T x0, T y0, T x1, T y1);
    Rect4(const Rect4& rect);
    explicit Rect4(T v);
    Rect4();

    bool        Intersects(const Rect4& rect) const;
    Rect4<T>    Intersection(const Rect4& rect) const;
    bool        Contains(const Rect4& rect) const;
    bool        Contains(T x, T y) const;
    T           GetWidth() const;
    T           GetHeight() const;

    bool        operator ==(const Rect4& rect) const;
    bool        operator !=(const Rect4& rect) const;
    Rect4<T>&   operator =(const Rect4& rect);
    Rect4<T>    operator +(const Rect4& rect) const;
    Rect4<T>    operator -(const Rect4& rect) const;
    Rect4<T>    operator *(T scalar) const;
    Rect4<T>    operator /(T scalar) const;
    Rect4<T>&   operator +=(const Rect4& rect);
    Rect4<T>&   operator -=(const Rect4& rect);
    Rect4<T>&   operator *=(T scalar);
    Rect4<T>&   operator /=(T scalar);

    T           x0;
    T           y0;
    T           x1;
    T           y1;
};

typedef Rect4<int>      Rect4i;
typedef Rect4<float>    Rect4f;

#include "Rect.inl"
