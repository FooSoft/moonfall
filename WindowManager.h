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
#include "IEventHandler.h"

class WindowManager : public IEventHandler
{
public:
    WindowManager();
    ~WindowManager();

    void                        Update(float elapsed, const Vector2i& resolution);
    void                        Render(const boost::shared_ptr<Surface>& target);

    void                        EnqueueWindow(const boost::shared_ptr<Window>& window);

private:
    enum FadeState
    {
        FADE_STATE_IN,
        FADE_STATE_OUT,
        FADE_STATE_NONE
    };

    void                        OnKeyDown(const InputKey& args);
    void                        OnKeyUp(const InputKey& args);
    void                        OnMouseMove(const InputMouse& args);
    void                        OnMouseButtonDown(const InputMouse& args);
    void                        OnMouseButtonUp(const InputMouse& args);
    void                        OnMouseWheel(const InputMouse& args);
    void                        OnActivate();
    void                        OnDeactivate();
    void                        OnQuit();

    boost::shared_ptr<Window>   m_activeWindow;
    boost::shared_ptr<Window>   m_queuedWindow;

    boost::shared_ptr<Surface>  m_fadeSurface;
    FadeState                   m_fadeState;
    float                       m_fadeAlpha;
};
