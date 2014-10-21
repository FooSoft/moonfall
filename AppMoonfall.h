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
#include "Driver.h"
#include "LuaBinding.h"
#include "FrameConsole.h"
#include "IEventHandler.h"

class FrameMapEditor;
class FrameSpriteEditor;
class DialogAssetPackager;

class AppMoonfall : public wxApp, public IEventHandler, public FrameConsole::IListener, public Trace::ITarget
{
public:
    AppMoonfall();
    ~AppMoonfall();

private:
    bool                    OnInit();
    bool                    ProcessIdle();
    int                     OnExit();

    void                    Trace(Trace::Level level, const char* text, const char* context);

    void                    OnKeyDown(const InputKey& args);
    void                    OnKeyUp(const InputKey& args) { }
    void                    OnMouseMove(const InputMouse& args) { }
    void                    OnMouseButtonDown(const InputMouse& args) { }
    void                    OnMouseButtonUp(const InputMouse& args) { }
    void                    OnMouseWheel(const InputMouse& args) { }
    void                    OnActivate() { }
    void                    OnDeactivate() { }
    void                    OnQuit();

    void                    OnConsoleInput(FrameConsole* console, const wxString& text);
    bool                    OnConsoleClose(FrameConsole* console);

    DialogAssetPackager*    m_dialogAssetPackager;
    FrameSpriteEditor*      m_frameSpriteEditor;
    FrameMapEditor*         m_frameMapEditor;
    FrameConsole*           m_frameConsole;

    LuaBinding              m_binding;
    Driver                  m_driver;
    bool                    m_done;
};
