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

class Token
{
public:
    Token(const char* string);
    Token(const Token& other);
    Token(int token);
    Token();

    bool    IsValid() const;

    bool    operator ==(const Token& other) const;
            operator int() const;

private:
    int     m_token;
};
