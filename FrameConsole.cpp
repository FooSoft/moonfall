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
#include "FrameConsole.h"

BEGIN_EVENT_TABLE(FrameConsole, wxFrame)
    EVT_CLOSE(                      FrameConsole::OnClose)
    EVT_ACTIVATE(                   FrameConsole::OnActivate)
    EVT_TEXT_ENTER(ID_TEXT_INPUT,   FrameConsole::OnTextEnter)
END_EVENT_TABLE()

FrameConsole::FrameConsole(wxWindow* parent, IListener* listener) :
        wxFrame(parent, wxID_ANY, wxT("Console"), wxDefaultPosition, wxSize(800, 600)),
        m_textOutput(NULL),
        m_textInput(NULL),
        m_listener(listener),
        m_historyIndex(0)
{
    m_textOutput = new wxTextCtrl(this, ID_TEXT_OUTPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_AUTO_SCROLL | wxTE_RICH2);
    m_textOutput->SetBackgroundColour(wxColor(0x00, 0x00, 0x00));
    m_textOutput->SetForegroundColour(wxColor(0xff, 0xff, 0xff));
    m_textOutput->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    m_textInput = new wxTextCtrl(this, ID_TEXT_INPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_textInput->SetBackgroundColour(wxColor(0x00, 0x00, 0x00));
    m_textInput->SetForegroundColour(wxColor(0xff, 0xff, 0xff));
    m_textInput->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    wxBoxSizer* bSizerMain = new wxBoxSizer(wxVERTICAL);
    bSizerMain->Add(m_textOutput, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);
    bSizerMain->Add(m_textInput, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    SetSizer(bSizerMain);

    m_textInput->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(FrameConsole::OnTextKeyDown), NULL, this);
}

void FrameConsole::OnClose(wxCloseEvent& event)
{
    if (m_listener != NULL && m_listener->OnConsoleClose(this))
    {
        event.Skip();
    }
    else
    {
        Hide();
    }
}

void FrameConsole::OnActivate(wxActivateEvent&)
{
    m_textInput->SetFocus();
}

void FrameConsole::OnTextKeyDown(wxKeyEvent& evt)
{
    switch (evt.GetKeyCode())
    {
    case WXK_UP:
        FlipTextHistory(false);
        break;
    case WXK_DOWN:
        FlipTextHistory(true);
        break;
    default:
        evt.Skip();
        break;
    }
}

void FrameConsole::FlipTextHistory(bool forward)
{
    if (m_history.size() == 0)
    {
        return;
    }

    if (forward)
    {
        m_historyIndex = std::min(++m_historyIndex, static_cast<int>(m_history.size() - 1));
        m_textInput->SetValue(m_history[m_historyIndex]);
    }
    else
    {
        m_historyIndex = std::max(--m_historyIndex, 0);
        m_textInput->SetValue(m_history[m_historyIndex]);
    }
}

void FrameConsole::OnTextEnter(wxCommandEvent& event)
{
    const wxString text = m_textInput->GetValue();
    if (text.IsEmpty())
    {
        return;
    }

    if (m_listener != NULL)
    {
        m_listener->OnConsoleInput(this, text);
    }

    m_textInput->Clear();

    if (m_history.size() == 0 || m_history[m_history.size() - 1] != text)
    {
        m_history.push_back(text);
    }

    m_historyIndex = m_history.size();
}

void FrameConsole::Write(const wxString& text, const wxColor& color)
{
    m_textOutput->SetDefaultStyle(wxTextAttr(color));
    m_textOutput->AppendText(text);
}

void FrameConsole::Clear()
{
    m_textOutput->Clear();
}
