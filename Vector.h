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
struct Vector2;

template <typename T>
struct VectorMixMath
{
    Vector2<T>  Normalized() const;
    Vector2<T>& Normalize();
    T           GetLength() const;
};

struct VectorMixDefault { };

template <typename T>
struct VectorMixConfig
{
    typedef VectorMixDefault Mix;
};

template <>
struct VectorMixConfig<float>
{
    typedef VectorMixMath<float> Mix;
};

template <>
struct VectorMixConfig<double>
{
    typedef VectorMixMath<double> Mix;
};

template <typename T>
struct Vector2 : public VectorMixConfig<T>::Mix
{
    Vector2(const Vector2& vector);
    Vector2(T x, T y);
    explicit Vector2(T v);
    Vector2();

    bool        operator ==(const Vector2& vector) const;
    bool        operator !=(const Vector2& vector) const;
    Vector2     operator +(const Vector2& vector) const;
    Vector2     operator -(const Vector2& vector) const;
    Vector2     operator *(T scalar) const;
    Vector2     operator /(T scalar) const;
    Vector2&    operator +=(const Vector2& vector);
    Vector2&    operator -=(const Vector2& vector);
    Vector2&    operator *=(T scalar);
    Vector2&    operator /=(T scalar);
    Vector2&    operator =(const Vector2& vector);
    Vector2     operator -() const;

    T           x;
    T           y;
};

typedef Vector2<int>    Vector2i;
typedef Vector2<float>  Vector2f;
typedef Vector2<double> Vector2d;

#include "Vector.inl"
