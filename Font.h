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

class Font
{
public:
    Font(TTF_Font* font, const Color4b& color);
    ~Font();

    boost::shared_ptr<Surface>  RenderText(const char* text) const;
    Vector2i                    MeasureText(const char* text) const;
    void                        MeasureGlyph(int glyph, Rect4i* rect, int* advance) const;

    void                        SetStyle(int style);
    int                         GetStyle() const;

    bool                        IsFixedWidth() const;
    int                         GetHeight() const;
    int                         GetAscent() const;
    int                         GetDescent() const;
    int                         GetLineSkip() const;

private:
    TTF_Font*                   m_font;
    Color4b                     m_color;
};
