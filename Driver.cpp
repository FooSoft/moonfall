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
#include "Driver.h"

Driver::Driver() :
        m_lastTick(0),
        m_sdlInitialized(false),
        m_ttfInitialized(false)
{
}

Driver::~Driver()
{
    Shutdown();
}

bool Driver::Initialize (const char* title)
{
    if (!m_sdlInitialized)
    {
        m_sdlInitialized = SDL_Init(SDL_INIT_EVERYTHING) == 0;
    }

    if (m_sdlInitialized && !m_ttfInitialized)
    {
        m_ttfInitialized = TTF_Init() == 0;
    }

    if (!m_sdlInitialized || !m_ttfInitialized)
    {
        TRACE_ERROR("SDL initialization failed");
        Shutdown();
        return false;
    }

    srand(static_cast<unsigned>(time(NULL)));
    SDL_WM_SetCaption(title, title);
    m_lastTick = SDL_GetTicks() - 1;

    TRACE_INFO("SDL initialized");
    return true;
}

void Driver::Shutdown()
{
    System::Reset();

    if (m_ttfInitialized)
    {
        TTF_Quit();
        m_ttfInitialized = false;
    }

    if (m_sdlInitialized)
    {
        SDL_Quit();
        m_sdlInitialized = false;
    }

    TRACE_INFO("SDL shutdown");
}

void Driver::Update()
{
    if (!m_sdlInitialized || !m_ttfInitialized)
    {
        return;
    }

    const int currentTick = SDL_GetTicks();
    const int tickDelta = MIN(currentTick - m_lastTick, 100);
    m_lastTick = currentTick;

#if 0
    static int s_count = 0;
    if (++s_count == 50)
    {
        TRACE_INFO(boost::format("%d") % (1000 / tickDelta));
        s_count = 0;
    }
#endif

    System::UpdateWindow(static_cast<float>(tickDelta) / 1000);
    System::RenderWindow();
}
