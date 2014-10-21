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
#include "EventManager.h"
#include "IEventHandler.h"

void EventManager::AddHandler(IEventHandler* handler)
{
    m_handlers.push_back(handler);
}

void EventManager::RemoveHandler(IEventHandler* handler)
{
    m_handlers.remove(handler);
}

bool EventManager::HasHandler(const IEventHandler* handler) const
{
    for (HandlerList::const_iterator iter = m_handlers.begin(); iter != m_handlers.end(); ++iter)
    {
        if (*iter == handler)
        {
            return true;
        }
    }

    return false;
}

void EventManager::Update(float)
{
    SDL_Event event = {0};

    while (SDL_PollEvent(&event))
    {
        for (HandlerList::iterator i = m_handlers.begin(); i != m_handlers.end(); i++)
        {
            IEventHandler* handler = *i;

            switch (event.type)
            {
            case SDL_ACTIVEEVENT:
                if (event.active.state == SDL_APPINPUTFOCUS)
                {
                    if (event.active.gain)
                    {
                        handler->OnActivate();
                    }
                    else
                    {
                        handler->OnDeactivate();
                    }
                }
                break;
            case SDL_KEYDOWN:
                handler->OnKeyDown(
                    InputKey(
                        event.key.keysym.sym,
                        event.key.keysym.mod,
                        event.key.keysym.scancode,
                        event.key.keysym.unicode
                    )
                );
                break;
            case SDL_KEYUP:
                handler->OnKeyUp(
                    InputKey(
                        event.key.keysym.sym,
                        event.key.keysym.mod,
                        event.key.keysym.scancode,
                        event.key.keysym.unicode
                    )
                );
                break;
            case SDL_MOUSEMOTION:
                handler->OnMouseMove(
                    InputMouse(
                        0,
                        event.motion.state,
                        Vector2i(event.motion.x, event.motion.y),
                        Vector2i(event.motion.xrel, event.motion.yrel),
                        0,
                        BuildKeyState()
                    )
                );
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_WHEELUP || event.button.button == SDL_BUTTON_WHEELDOWN)
                {
                    handler->OnMouseWheel(
                        InputMouse(
                            0,
                            event.button.state,
                            Vector2i(event.button.x, event.button.y),
                            Vector2i(0, 0),
                            event.button.button == SDL_BUTTON_WHEELUP ? 1 : -1,
                            BuildKeyState()
                        )
                    );
                }
                else
                {
                    handler->OnMouseButtonDown(
                        InputMouse(
                            event.button.button,
                            event.button.state,
                            Vector2i(event.button.x, event.button.y),
                            Vector2i(0, 0),
                            0,
                            BuildKeyState()
                        )
                    );
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button != SDL_BUTTON_WHEELUP && event.button.button != SDL_BUTTON_WHEELDOWN)
                {
                    handler->OnMouseButtonUp(
                        InputMouse(
                            event.button.button,
                            event.button.state,
                            Vector2i(event.button.x, event.button.y),
                            Vector2i(0, 0),
                            0,
                            BuildKeyState()
                        )
                    );
                }
                break;
            case SDL_QUIT:
                handler->OnQuit();
                break;
            }
        }
    }
}

int EventManager::BuildKeyState()
{
    return
        wxGetKeyState(WXK_SHIFT) ? KMOD_SHIFT : 0 |
        wxGetKeyState(WXK_ALT) ? KMOD_ALT : 0 |
        wxGetKeyState(WXK_CONTROL) ? KMOD_CTRL : 0;
}
