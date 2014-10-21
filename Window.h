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

class Window : public IEventHandler
{
public:
    Window();
    virtual ~Window() { }

    void            Render(const boost::shared_ptr<Surface>& target);
    void            Update(float elapsed);

    void            SetPosition(const Vector2i& position);
    Vector2i        GetPosition() const;
    void            SetSize(const Vector2i& size);
    Vector2i        GetSize() const;
    void            SetBounds(const Rect4i& bounds);
    Rect4i          GetBounds() const;

    void            AddChild(const boost::shared_ptr<Window>& window);
    void            RemoveChild(const boost::shared_ptr<Window>& window);
    void            ClearChildren();

    void            SetDone();
    bool            IsDone() const;

    virtual void    OnRender(const boost::shared_ptr<Surface>& target);
    virtual void    OnUpdate(float elapsed);
    virtual void    OnKeyDown(const InputKey& args);
    virtual void    OnKeyUp(const InputKey& args);
    virtual void    OnMouseMove(const InputMouse& args);
    virtual void    OnMouseButtonDown(const InputMouse& args);
    virtual void    OnMouseButtonUp(const InputMouse& args);
    virtual void    OnMouseWheel(const InputMouse& args);
    virtual void    OnActivate();
    virtual void    OnDeactivate();
    virtual void    OnLoad();
    virtual void    OnUnload();
    virtual void    OnQuit();

private:
    bool            AdjustMouseArgs(const boost::shared_ptr<Window>& window, const InputMouse& sourceArgs, InputMouse* destArgs) const;

    typedef         std::vector<boost::shared_ptr<Window> > WindowList;

    WindowList      m_children;
    Vector2i        m_position;
    Vector2i        m_size;
    bool            m_done;
};
