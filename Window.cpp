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
#include "Window.h"
#include "Surface.h"

Window::Window() :
        m_done(false)
{
}

void Window::Render(const boost::shared_ptr<Surface>& target)
{
    if (IsDone())
    {
        return;
    }

    target->PushState();
    target->SetViewportRelative(GetBounds());
    OnRender(target);
    target->PopState();
}

void Window::Update(float elapsed)
{
    if (IsDone())
    {
        return;
    }

    OnUpdate(elapsed);

    WindowList doneChildren;
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        const boost::shared_ptr<Window> child(*iter);
        if (child->IsDone())
        {
            doneChildren.push_back(child);
        }
    }

    for (WindowList::iterator iter = doneChildren.begin(); iter != doneChildren.end(); ++iter)
    {
        m_children.erase(find(m_children.begin(), m_children.end(), *iter));
    }
}

void Window::SetPosition(const Vector2i& position)
{
    m_position = position;
}

Vector2i Window::GetPosition() const
{
    return m_position;
}

void Window::SetSize(const Vector2i& size)
{
    m_size = size;
}

Vector2i Window::GetSize() const
{
    return m_size;
}

void Window::SetBounds(const Rect4i& bounds)
{
    SetPosition(Vector2i(bounds.x0, bounds.y0));
    SetSize(Vector2i(bounds.x1 - bounds.x0, bounds.y1 - bounds.y0));
}

Rect4i Window::GetBounds() const
{
    return Rect4i(
               GetPosition().x,
               GetPosition().y,
               GetPosition().x + GetSize().x,
               GetPosition().y + GetSize().y
           );
}

void Window::AddChild(const boost::shared_ptr<Window>& window)
{
    m_children.push_back(window);
    window->OnLoad();
}

void Window::RemoveChild(const boost::shared_ptr<Window>& window)
{
    WindowList::iterator iter = find(m_children.begin(), m_children.end(), window);
    if (iter != m_children.end())
    {
        (*iter)->OnUnload();
        m_children.erase(iter);
    }
}

void Window::ClearChildren()
{
    m_children.clear();
}

void Window::SetDone()
{
    m_done = true;
}

bool Window::IsDone() const
{
    return m_done;
}

void Window::OnRender(const boost::shared_ptr<Surface>& target)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->Render(target);
    }
}

void Window::OnUpdate(float elapsed)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->Update(elapsed);
    }
}

void Window::OnKeyDown(const InputKey& args)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnKeyDown(args);
    }
}

void Window::OnKeyUp(const InputKey& args)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnKeyUp(args);
    }
}

void Window::OnMouseMove(const InputMouse& args)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        InputMouse adjustedArgs(args);
        boost::shared_ptr<Window> child(*iter);
        if (AdjustMouseArgs(child, args, &adjustedArgs))
        {
            child->OnMouseMove(adjustedArgs);
        }
    }
}

void Window::OnMouseButtonDown(const InputMouse& args)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        InputMouse adjustedArgs(args);
        boost::shared_ptr<Window> child(*iter);
        if (AdjustMouseArgs(child, args, &adjustedArgs))
        {
            child->OnMouseButtonDown(adjustedArgs);
        }
    }
}

void Window::OnMouseButtonUp(const InputMouse& args)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        InputMouse adjustedArgs(args);
        boost::shared_ptr<Window> child(*iter);
        if (AdjustMouseArgs(child, args, &adjustedArgs))
        {
            child->OnMouseButtonUp(adjustedArgs);
        }
    }
}

void Window::OnMouseWheel(const InputMouse& args)
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        InputMouse adjustedArgs(args);
        boost::shared_ptr<Window> child(*iter);
        if (AdjustMouseArgs(child, args, &adjustedArgs))
        {
            child->OnMouseWheel(adjustedArgs);
        }
    }
}

void Window::OnActivate()
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnActivate();
    }
}

void Window::OnDeactivate()
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnDeactivate();
    }
}

void Window::OnLoad()
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnLoad();
    }
}

void Window::OnUnload()
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnUnload();
    }
}

void Window::OnQuit()
{
    for (WindowList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        (*iter)->OnQuit();
    }
}

bool Window::AdjustMouseArgs(const boost::shared_ptr<Window>& window, const InputMouse& sourceArgs, InputMouse* destArgs) const
{
    *destArgs = sourceArgs;
    destArgs->position = sourceArgs.position - window->GetPosition();
    return
        destArgs->position.x >= 0 &&
        destArgs->position.y >= 0 &&
        destArgs->position.x < window->GetSize().x &&
        destArgs->position.y < window->GetSize().y;
}
