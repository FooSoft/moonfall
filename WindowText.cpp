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
#include "WindowText.h"
#include "Surface.h"
#include "Font.h"

void WindowText::SetText(const char* text, const char* fontAlias, int maxWidth)
{
    const int textWidth = maxWidth == 0 ? GetSize().x : maxWidth;
    m_surface = RenderText(text, System::LoadFont(fontAlias), textWidth, &m_desiredSize);
}

Vector2i WindowText::GetDesiredSize() const
{
    return m_desiredSize;
}

void WindowText::OnRender(const boost::shared_ptr<Surface>& target)
{
    Window::OnRender(target);
    if (m_surface)
    {
        m_surface->Blit(target, Vector2i(0, 0), m_surface->GetSize());
    }
}

boost::shared_ptr<Surface> WindowText::RenderText(const char* text, const boost::shared_ptr<Font>& font, int maxWidth, Vector2i* desiredSize)
{
    if (maxWidth == 0 || !font)
    {
        return boost::shared_ptr<Surface>();
    }

    std::vector<Word> words;
    Word currentWord;

    for (int i = 0; text[i] != 0; ++i)
    {
        const char c = text[i];
        const bool isSpace = (c == ' ' || c == '\n' || c == '\t');

        if (!isSpace)
        {
            int glyphWidth = 0;
            font->MeasureGlyph(c, NULL, &glyphWidth);
            currentWord.text += c;
            currentWord.width += glyphWidth;
        }

        if ((isSpace || text[i + 1] == 0) && currentWord.width > 0)
        {
            words.push_back(currentWord);
            currentWord = Word();
        }
    }

    int spaceWidth = 0;
    font->MeasureGlyph(' ', NULL, &spaceWidth);

    std::vector<std::string> lines;
    std::string lineText;
    int lineWidth = 0;
    std::vector<Word>::iterator wordIter = words.begin();

    while (wordIter != words.end())
    {
        const int newLineWidth = lineWidth + wordIter->width + (lineWidth > 0 ? spaceWidth : 0);

        if (newLineWidth < maxWidth)
        {
            if (lineWidth > 0)
            {
                lineText += ' ';
            }

            lineText += wordIter->text;
            lineWidth = newLineWidth;
            ++wordIter;
        }

        if (newLineWidth >= maxWidth || wordIter == words.end())
        {
            lines.push_back(lineText);
            lineText.clear();
            lineWidth = 0;
        }
    }

    const int lineSkip = font->GetLineSkip();
    const boost::shared_ptr<Surface> textSurface = System::CreateSurface(Vector2i(maxWidth, lines.size() * lineSkip), COLOR_MODE_RGBA_8888);
    desiredSize->y = textSurface->GetSize().y;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        const boost::shared_ptr<Surface> lineSurface = font->RenderText(lines[i].c_str());
        lineSurface->SetAlpha(0, 0);
        textSurface->SetTranslation(Vector2i(0, i * lineSkip));
        lineSurface->Blit(textSurface, Vector2i(0, 0), lineSurface->GetSize());
        desiredSize->x = MAX(desiredSize->x, lineSurface->GetSize().x);
    }

    return textSurface;
}
