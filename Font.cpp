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
#include "Font.h"
#include "Surface.h"

Font::Font(TTF_Font* font, const Color4b& color) :
        m_font(font),
        m_color(color)
{
}

Font::~Font()
{
    TTF_CloseFont(m_font);
}

boost::shared_ptr<Surface> Font::RenderText(const char* text) const
{
    SDL_Color color = {0};
    color.r = m_color.red;
    color.g = m_color.green;
    color.b = m_color.blue;
    return boost::shared_ptr<Surface>(new Surface(TTF_RenderText_Blended(m_font, text, color)));
}

Vector2i Font::MeasureText(const char* text) const
{
    Vector2i size;
    TTF_SizeText(m_font, text, &size.x, &size.y);
    return size;
}

void Font::MeasureGlyph(int glyph, Rect4i* rect, int* advance) const
{
    Rect4i temp;
    TTF_GlyphMetrics(m_font, glyph, &temp.x0, &temp.x1, &temp.y0, &temp.y1, advance);
    if (rect != NULL)
    {
        *rect = temp;
    }
}

void Font::SetStyle(int style)
{
    TTF_SetFontStyle(m_font, style);
}

int Font::GetStyle() const
{
    return TTF_GetFontStyle(m_font);
}

bool Font::IsFixedWidth() const
{
    return TTF_FontFaceIsFixedWidth(m_font) > 0;
}

int Font::GetHeight() const
{
    return TTF_FontHeight(m_font);
}

int Font::GetAscent() const
{
    return TTF_FontAscent(m_font);
}

int Font::GetDescent() const
{
    return TTF_FontDescent(m_font);
}

int Font::GetLineSkip() const
{
    return TTF_FontLineSkip(m_font);
}
