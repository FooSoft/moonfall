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

class WindowDialogue : public Window
{
public:
    void                                PushScreen(const char* name);
    void                                PushScreenPanel(const char* text, const char* title, const char* sprite, const char* jump);
    void                                PushScreenPanelOption(const char* text, const char* value, const char* jump);

private:
    struct ScreenPanelOption
    {
        ScreenPanelOption(const char* text, const char* value, const char* jump) :
                text(text == NULL ? "" : text),
                value(value == NULL ? "" : value) { }

        std::string                     text;
        std::string                     value;
        std::string                     jump;
    };

    struct ScreenPanel
    {
        ScreenPanel(const char* text, const char* title, const char* sprite, const char* jump) :
                text(text == NULL ? "" : text),
                title(title == NULL ? "" : title),
                sprite(sprite == NULL ? "" : sprite),
                jump(jump == NULL ? "" : jump) { }

        std::string                     text;
        std::string                     title;
        std::string                     sprite;
        std::string                     jump;
        std::vector<ScreenPanelOption>  options;
    };

    struct Screen
    {
        Screen(const char* name) :
                name(name == NULL ? "" : name) { }

        std::string                     name;
        std::vector<ScreenPanel>        panels;
    };

    void                                OnRender(const boost::shared_ptr<Surface>& target);
    void                                OnMouseMove(const InputMouse& args);
    void                                OnKeyDown(const InputKey& args);
    void                                OnLoad();

    boost::shared_ptr<Surface>          m_backdrop;
    std::vector<Screen>                 m_screens;
};
