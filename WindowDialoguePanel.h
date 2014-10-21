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
#include "Window.h"

class WindowDialoguePanel : public Window
{
public:
    void                        SetPanel(const char* text, const char* textFontAlias, const char* title, const char* titleFontAlias, const char* spriteAlias, int padding, int maxWidth);
    Vector2i                    GetDesiredSize() const;

private:
    void                        OnRender(const boost::shared_ptr<Surface>& target);

    boost::shared_ptr<Sprite>   m_sprite;
    Vector2i                    m_spritePosition;
    Vector2i                    m_desiredSize;
};
