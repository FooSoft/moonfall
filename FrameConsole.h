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

class FrameConsole : public wxFrame
{
public:
    class IListener
    {
    public:
        virtual void OnConsoleInput(FrameConsole* console, const wxString& text) = 0;
        virtual bool OnConsoleClose(FrameConsole* console) = 0;

    protected:
        ~IListener() {}
    };

    FrameConsole(wxWindow* parent, IListener* listener);

    void Write(const wxString& text, const wxColor& color = wxColor(0xff, 0xff, 0xff));
    void Clear();

private:
    enum
    {
        ID_TEXT_OUTPUT,
        ID_TEXT_INPUT
    };

    void OnClose(wxCloseEvent& event);
    void OnActivate(wxActivateEvent& event);
    void OnTextEnter(wxCommandEvent& event);
    void OnTextKeyDown(wxKeyEvent& evt);
    void FlipTextHistory(bool forward);

    DECLARE_EVENT_TABLE()

    wxTextCtrl*             m_textOutput;
    wxTextCtrl*             m_textInput;
    std::vector<wxString>   m_history;
    IListener*              m_listener;
    int                     m_historyIndex;
};
