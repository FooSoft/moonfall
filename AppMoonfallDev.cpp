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
#include "DialogAssetPackager.h"
#include "FrameConsole.h"
#include "FrameMapEditor.h"
#include "FrameSpriteEditor.h"

void AppMoonfall::OnKeyDown(const InputKey& args)
{
    if (args.key == '`')
    {
        m_frameConsole->Show();
        m_frameConsole->Raise();
    }
}

void AppMoonfall::OnConsoleInput(FrameConsole* console, const wxString& text)
{
    const wxColor echoColor(0x80, 0x80, 0x80);
    const wxString command = text.BeforeFirst(' ');

    console->Write(wxT("> ") + text + wxT("\n"), echoColor);

    if (command == wxT("/clear"))
    {
        console->Clear();
    }
    else if (command == wxT("/quit"))
    {
        ExitMainLoop();
    }
    else if (command == wxT("/help"))
    {
        console->Write(
            wxT("/help\n")
#ifdef DEVELOPER
            wxT("/edit [sprites | assets | maps]\n")
            wxT("/show [layer | shapes | backdrop]\n")
            wxT("/hide [layer | shapes | backdrop]\n")
            wxT("/lua [code]\n")
            wxT("/flush\n")
            wxT("/vis\n")
#endif
            wxT("/clear\n")
            wxT("/quit\n"),
            echoColor
        );
    }
#ifdef DEVELOPER
    else if (command == wxT("/flush"))
    {
        System::FlushFiles();
        System::FlushSurfaces();
    }
    else if (command == wxT("/lua"))
    {
        const wxString parameter = text.AfterFirst(' ');
        m_binding.DoString(parameter.ToAscii());
    }
    else if (command == wxT("/edit"))
    {
        const wxString type = text.AfterFirst(' ');
        if (type == wxT("sprites"))
        {
            if (m_frameSpriteEditor == NULL)
            {
                m_frameSpriteEditor = new FrameSpriteEditor(NULL, wxT("Assets/SpriteManifest.xml"));
            }
            m_frameSpriteEditor->Show();
            m_frameSpriteEditor->Raise();
        }
        else if (type == wxT("assets"))
        {
            if (m_dialogAssetPackager == NULL)
            {
                m_dialogAssetPackager = new DialogAssetPackager(NULL, wxT("Assets"), wxT("Assets.pak"));
            }
            m_dialogAssetPackager->Show();
            m_dialogAssetPackager->Raise();
        }
        else if (type == wxT("maps"))
        {
            if (m_frameMapEditor == NULL)
            {
                m_frameMapEditor = new FrameMapEditor(NULL);
            }
            m_frameMapEditor->Show();
            m_frameMapEditor->Raise();
        }
    }
    else if (command == wxT("/vis"))
    {
        IWorldContext * const context = System::QueryWorldContext();
        if (context == NULL)
        {
            return;
        }

        const wchar_t* visibility[] = { L"hidden", L"visible" };

        console->Write(wxString::Format(wxT("Backdrop: %s\n"), visibility[context->IsBackdropSpriteVisible()]), echoColor);
        for (int i = 0; i < ACTOR_LAYER_COUNT; ++i)
        {
            console->Write(wxString::Format(wxT("Layer %d: %s\n"), i, visibility[context->IsLayerVisible(static_cast<const ActorLayer>(i))]), echoColor);
        }
        console->Write(wxString::Format(wxT("Shapes: %s\n"), visibility[context->AreShapesVisible()]), echoColor);
    }
    else if (command == wxT("/show") || command == wxT("/hide"))
    {
        IWorldContext * const context = System::QueryWorldContext();
        if (context == NULL)
        {
            return;
        }

        const wxString remainder = text.AfterFirst(' ');
        const wxString type = remainder.BeforeFirst(' ');
        const bool show = command == wxT("/show");

        if (type == wxT("layer"))
        {
            long layer = 0;
            if (!remainder.AfterFirst(' ').ToLong(&layer))
            {
                return;
            }

            if (show)
            {
                context->ShowLayer(static_cast<ActorLayer>(layer));
            }
            else
            {
                context->HideLayer(static_cast<ActorLayer>(layer));
            }
        }
        else if (type == wxT("shapes"))
        {
            if (show)
            {
                context->ShowShapes();
            }
            else
            {
                context->HideShapes();
            }
        }
        else if (type == wxT("backdrop"))
        {
            if (show)
            {
                context->ShowBackdropSprite();
            }
            else
            {
                context->HideBackdropSprite();
            }
        }
    }
#endif
    else
    {
        console->Write(wxT("Unrecognized command ") + command + wxT("\n"), echoColor);
    }
}

bool AppMoonfall::OnConsoleClose(FrameConsole* console)
{
    return false;
}

void AppMoonfall::Trace(Trace::Level level, const char* text, const char* context)
{
    static const wxColor s_colorLocation(0xff, 0xff, 0xff);
    static const wxColor s_colors[] =
    {
        wxColor(0x00, 0xff, 0x00),
        wxColor(0xff, 0xff, 0x00),
        wxColor(0xff, 0x00, 0x00)
    };

    ASSERT(level < ARRAY_SIZE(s_colors));

    if (m_frameConsole != NULL)
    {
        if (context != NULL)
        {
            m_frameConsole->Write(wxString::FromAscii(context) + wxT(" - "), s_colorLocation);
        }
        m_frameConsole->Write(wxString::FromAscii(text) + wxT("\n"), s_colors[level]);
    }

#ifdef DEVELOPER
    if (m_frameConsole != NULL && level >= Trace::LEVEL_WARNING)
    {
        m_frameConsole->Show();
    }
#endif
}
