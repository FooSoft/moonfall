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
#include "AppMoonfall.h"
#include "FrameConsole.h"
#include "WindowMain.h"

AppMoonfall::AppMoonfall() :
        m_dialogAssetPackager(NULL),
        m_frameSpriteEditor(NULL),
        m_frameMapEditor(NULL),
        m_frameConsole(NULL),
        m_done(false)
{
}

AppMoonfall::~AppMoonfall()
{
#ifdef HEAP_TRACK
    if (Heap::HasAllocs())
    {
        TRACE_WARNING("Memory leaks detected");

        std::vector<Heap::Block> blocks;
        Heap::DumpAllocs(&blocks);

        for (std::vector<Heap::Block>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
        {
            TRACE_WARNING(
                boost::format("Block:\n\tAddress = %p\n\tSize = %d\n\tFilename = %s\n\tLine =  %d") % iter->address % iter->size % iter->filename % iter->line
            );
        }
    }

    ASSERT(!Heap::HasAllocs());
    Heap::TrackEnd();
#endif

    System::RemoveEventHandler(this);
    Trace::RemoveTarget(this);
}

bool AppMoonfall::OnInit()
{
#ifdef HEAP_TRACK
    Heap::TrackBegin();
#endif
    wxInitAllImageHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    void InitXmlResource();
    InitXmlResource();

    m_frameConsole = new FrameConsole(NULL, this);
    m_done = false;

    Trace::AddTarget(this);
    System::AddEventHandler(this);

    const Vector2i size(800, 600);

#ifdef DEVELOPER
    const char title[] = "Moonfall - Developer";
#else
    const char title[] = "Moonfall";
#endif

#ifdef WINDOWS
    wxFrame* frameSdl = new wxFrame(
        NULL,
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION
    );
    frameSdl->SetClientSize(size.x, size.y);
    frameSdl->Show();
    static char s_id[32] = {0};
    sprintf(s_id, "%u", frameSdl->GetHandle());
    SetEnvironmentVariableA("SDL_WINDOWID", s_id);
#endif

    if (!m_driver.Initialize(title))
    {
        return false;
    }

    System::CreateFramebufferSurface(size, COLOR_MODE_RGB_888);
    System::EnqueueWindow(boost::shared_ptr<Window>(new WindowMain()));
    return true;
}

int AppMoonfall::OnExit()
{
    m_driver.Shutdown();

    // all windows are destroyed by the time execution enters
    // the destructor, so clear these would-be dangling pointers
    m_dialogAssetPackager = NULL;
    m_frameConsole = NULL;
    m_frameMapEditor = NULL;

    return 0;
}

bool AppMoonfall::ProcessIdle()
{
    wxApp::ProcessIdle();
    m_driver.Update();

    if (m_done)
    {
        ExitMainLoop();
    }

    return !m_done;
}

void AppMoonfall::OnQuit()
{
    m_done = true;
}

IMPLEMENT_APP(AppMoonfall)
