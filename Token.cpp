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

#include "Pch.h"
#include "Token.h"

Token::Token(const char* string) :
        m_token(5381)
{
    unsigned& token = reinterpret_cast<unsigned&>(m_token);
    char letter = 0;

    while ((letter = *string++) != 0)
    {
        token = (token << 5) + token + letter;
    }
}

Token::Token(const Token& other) :
        m_token(other.m_token)
{
}

Token::Token(int token) :
        m_token(token)
{
}

Token::Token() :
        m_token(0)
{
}

bool Token::operator ==(const Token& other) const
{
    return m_token == other.m_token;
}

Token::operator int() const
{
    return m_token;
}

bool Token::IsValid() const
{
    return m_token != 0;
}
