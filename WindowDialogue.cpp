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
#include "WindowDialogue.h"
#include "WindowDialoguePanel.h"
#include "Surface.h"

void WindowDialogue::PushScreen(const char* name)
{
    m_screens.push_back(Screen(name));
}

void WindowDialogue::PushScreenPanel(const char* text, const char* title, const char* sprite, const char* jump)
{
    const size_t screenCount = m_screens.size();
    ASSERT(screenCount > 0);
    m_screens[screenCount - 1].panels.push_back(ScreenPanel(text, title, sprite, jump));
}

void WindowDialogue::PushScreenPanelOption(const char* text, const char* value, const char* jump)
{
    const size_t screenCount = m_screens.size();
    ASSERT(screenCount > 0);
    const size_t screenPanelCount = m_screens[screenCount - 1].panels.size();
    ASSERT(screenPanelCount > 0);
    m_screens[screenCount - 1].panels[screenPanelCount - 1].options.push_back(ScreenPanelOption(text, value, jump));
}

void WindowDialogue::OnRender(const boost::shared_ptr<Surface>& target)
{
    m_backdrop->Blit(target, Vector2i(0, 0), GetSize());
    Window::OnRender(target);
}

void WindowDialogue::OnMouseMove(const InputMouse& args)
{
    Window::OnMouseMove(args);
    if (args.buttonState == SDL_BUTTON_LMASK)
    {
        SetPosition(GetPosition() + args.delta);
    }
}

void WindowDialogue::OnKeyDown(const InputKey& args)
{
    Window::OnKeyDown(args);
    if (args.key == SDLK_SPACE)
    {
        SetDone();
    }
}

void WindowDialogue::OnLoad()
{
    Window::OnLoad();

    m_backdrop = System::CreateSurface(GetSize(), COLOR_MODE_RGBA_8888);
    m_backdrop->Clear(Color4b(0x00, 0x00, 0xff, 0x80), GetSize());
}
