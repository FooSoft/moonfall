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
Vector2<T>::Vector2(const Vector2& vector) :
        x(0),
        y(0)
{
    *this = vector;
}

template <typename T>
Vector2<T>::Vector2(T x, T y) :
        x(x),
        y(y)
{
}

template <typename T>
Vector2<T>::Vector2(T v) :
        x(v),
        y(v)
{
}

template <typename T>
Vector2<T>::Vector2() :
        x(0),
        y(0)
{
}

template <typename T>
bool Vector2<T>::operator ==(const Vector2& vector) const
{
    return x == vector.x && y == vector.y;
}

template <typename T>
bool Vector2<T>::operator !=(const Vector2& vector) const
{
    return !(*this == vector);
}

template <typename T>
Vector2<T> Vector2<T>::operator +(const Vector2& vector) const
{
    return Vector2(x + vector.x, y + vector.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator -(const Vector2& vector) const
{
    return Vector2(x - vector.x, y - vector.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator *(T scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

template <typename T>
Vector2<T> Vector2<T>::operator /(T scalar) const
{
    return Vector2(x / scalar, y / scalar);
}

template <typename T>
Vector2<T>& Vector2<T>::operator +=(const Vector2& vector)
{
    *this = *this + vector;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator -=(const Vector2& vector)
{
    *this = *this - vector;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator *=(T scalar)
{
    *this = *this * scalar;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator /=(T scalar)
{
    *this = *this / scalar;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator =(const Vector2& vector)
{
    if (this != &vector)
    {
        x = vector.x;
        y = vector.y;
    }

    return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator -()const
{
    return Vector2(-x, -y);
}

template <typename T>
Vector2<T>& VectorMixMath<T>::Normalize()
{
    Vector2<T>* const vector = static_cast<Vector2<T> *>(this);

    const float length = vector->GetLength();
    if (length > 0)
    {
        *(vector) /= length;
    }
    else
    {
        *(vector) = Vector2<T>(0, 0);
    }

    return *vector;
}

template <typename T>
Vector2<T> VectorMixMath<T>::Normalized() const
{
    Vector2<T> vector = *static_cast<const Vector2<T> *>(this);
    vector.Normalize();
    return vector;
}

template <typename T>
T VectorMixMath<T>::GetLength() const
{
    const Vector2<T>* const vector = static_cast<const Vector2<T> *>(this);
    return static_cast<T>(sqrt(vector->x * vector->x + vector->y * vector->y));
}
