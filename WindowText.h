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

class WindowText : public Window
{
public:
    void                                SetText(const char* text, const char* fontAlias, int maxWidth = 0);
    Vector2i                            GetDesiredSize() const;

private:
    struct Word
    {
        Word() :
                width(0) { }

        std::string text;
        int         width;
    };

    void                                OnRender(const boost::shared_ptr<Surface>& target);
    static boost::shared_ptr<Surface>   RenderText(const char* text, const boost::shared_ptr<Font>& font, int maxWidth, Vector2i* desiredSize);

    boost::shared_ptr<Surface>          m_surface;
    Vector2i                            m_desiredSize;
};
