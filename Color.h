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
struct Color4
{
public:
    Color4(T red, T green, T blue, T alpha) :
            red(red),
            green(green),
            blue(blue),
            alpha(alpha)
    {
    }

    Color4(const Color4& color) :
            red(color.red),
            green(color.green),
            blue(color.blue),
            alpha(color.alpha)
    {
    }

    explicit Color4(T v) :
            red(v),
            green(v),
            blue(v),
            alpha(v)
    {
    }

    Color4() :
            red(0),
            green(0),
            blue(0),
            alpha(0)
    {
    }

    T red;
    T green;
    T blue;
    T alpha;
};

typedef Color4<byte>    Color4b;
typedef Color4<dword>   Color4d;
