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
#include "WindowManager.h"
#include "Window.h"
#include "Surface.h"

WindowManager::WindowManager() :
        m_fadeState(FADE_STATE_NONE),
        m_fadeAlpha(0)
{
    System::AddEventHandler(this);
}

WindowManager::~WindowManager()
{
    System::RemoveEventHandler(this);
}

void WindowManager::Update(float elapsed, const Vector2i& resolution)
{
    if (m_fadeState == FADE_STATE_IN)
    {
        m_fadeAlpha -= elapsed * 1000;
        m_fadeAlpha = MAX(m_fadeAlpha, 0);
        if (m_fadeAlpha == 0)
        {
            m_fadeState = FADE_STATE_NONE;
        }
    }
    else if (m_fadeState == FADE_STATE_OUT)
    {
        m_fadeAlpha += elapsed * 1000;
        m_fadeAlpha = MIN(m_fadeAlpha, 255);
        if (m_fadeAlpha == 255)
        {
            m_fadeState = FADE_STATE_IN;
        }
    }

    if (m_fadeState == FADE_STATE_OUT)
    {
        return;
    }

    if (m_queuedWindow)
    {
        if (m_activeWindow)
        {
            m_activeWindow->OnUnload();
        }

        m_activeWindow = m_queuedWindow;
        m_activeWindow->SetPosition(Vector2i(0, 0));
        m_activeWindow->SetSize(resolution);
        m_queuedWindow.reset();

        if (m_activeWindow)
        {
            m_activeWindow->OnLoad();
        }
    }

    if (m_activeWindow)
    {
        m_activeWindow->Update(elapsed);

        if (m_activeWindow->IsDone())
        {
            m_activeWindow.reset();
        }
    }
}

void WindowManager::Render(const boost::shared_ptr<Surface>& target)
{
    if (!m_activeWindow)
    {
        return;
    }

    m_activeWindow->Render(target);

    if (m_fadeState != FADE_STATE_NONE)
    {
        if (!m_fadeSurface || m_fadeSurface->GetSize() != target->GetSize())
        {
            m_fadeSurface = System::CreateSurface(target->GetSize(), COLOR_MODE_RGBA_8888);
        }

        m_fadeSurface->Clear(Color4b(0x00, 0x00, 0x00, static_cast<byte>(m_fadeAlpha)));
        m_fadeSurface->Blit(target, Vector2i(0, 0), target->GetSize());
    }

    target->Flip();
}

void WindowManager::EnqueueWindow(const boost::shared_ptr<Window>& window)
{
    m_queuedWindow = window;
    m_fadeState = m_activeWindow ? FADE_STATE_OUT : FADE_STATE_NONE;
    m_fadeAlpha = 0;
}

void WindowManager::OnKeyDown(const InputKey& args)
{
    if (m_activeWindow)
    {
        m_activeWindow->OnKeyDown(args);
    }
}

void WindowManager::OnKeyUp(const InputKey& args)
{
    if (m_activeWindow)
    {
        m_activeWindow->OnKeyUp(args);
    }
}

void WindowManager::OnMouseMove(const InputMouse& args)
{
    if (m_activeWindow)
    {
        m_activeWindow->OnMouseMove(args);
    }
}

void WindowManager::OnMouseButtonDown(const InputMouse& args)
{
    if (m_activeWindow)
    {
        m_activeWindow->OnMouseButtonDown(args);
    }
}

void WindowManager::OnMouseButtonUp(const InputMouse& args)
{
    if (m_activeWindow)
    {
        m_activeWindow->OnMouseButtonUp(args);
    }
}

void WindowManager::OnMouseWheel(const InputMouse& args)
{
    if (m_activeWindow)
    {
        m_activeWindow->OnMouseWheel(args);
    }
}

void WindowManager::OnActivate()
{
    if (m_activeWindow)
    {
        m_activeWindow->OnActivate();
    }
}

void WindowManager::OnDeactivate()
{
    if (m_activeWindow)
    {
        m_activeWindow->OnDeactivate();
    }
}

void WindowManager::OnQuit()
{
    if (m_activeWindow)
    {
        m_activeWindow->OnQuit();
        m_activeWindow.reset();
    }
}
