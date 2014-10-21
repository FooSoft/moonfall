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
#include "IWorldContext.h"

class FrameMapEditor : public wxFrame, public IWorldListener
{
public:
    FrameMapEditor(wxWindow* parent);

private:
    enum
    {
        ID_MENU_FILE_NEW,
        ID_MENU_FILE_OPEN,
        ID_MENU_FILE_SAVE,
        ID_MENU_FILE_SAVE_AS,
        ID_MENU_FILE_EXIT,

        ID_MENU_VIEW_GRID_HALVE,
        ID_MENU_VIEW_GRID_DOUBLE,
        ID_MENU_VIEW_GRID_CUSTOM,

        ID_WORLD_SCRIPT,
        ID_WORLD_SCRIPT_BROWSE,
        ID_WORLD_BACKDROP,
        ID_WORLD_BACKDROP_BROWSE,

        ID_PALETTE_LIST,
        ID_PALETTE_FILTER,

        ID_SELECTION_NAME,
        ID_SELECTION_POSITION_X,
        ID_SELECTION_POSITION_Y,
        ID_SELECTION_PREVIEW,
        ID_SELECTION_PARAMS
    };

    void        InitializeUiMenuBar();
    wxSizer*    InitializeUiWorld();
    wxSizer*    InitializeUiPalette();
    wxSizer*    InitializeUiSelection();

    void UpdateUiWorld();
    void UpdateUiPalette();
    void UpdateUiSelection();

    void OnModifiedUiWorld(wxCommandEvent& event);
    void OnModifiedUiPalette(wxCommandEvent& event);
    void OnModifiedUiSelection(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);
    void OnShow(wxShowEvent& event);

    void OnWorldKeyDown(const InputKey& args) {}
    void OnWorldKeyUp(const InputKey& args) {}
    void OnWorldMouseMove(const InputMouse& args);
    void OnWorldMouseButtonDown(const InputMouse& args);
    void OnWorldMouseButtonUp(const InputMouse& args) {}
    void OnWorldMouseWheel(const InputMouse& args) {}
    void OnWorldUpdate(float elapsed) {}
    void OnWorldActivate() {}
    void OnWorldDeactivate() {}

    void OnMenuSelectedFileNew(wxCommandEvent& event);
    void OnMenuSelectedFileOpen(wxCommandEvent& event);
    void OnMenuSelectedFileSave(wxCommandEvent& event);
    void OnMenuSelectedFileSaveAs(wxCommandEvent& event);
    void OnMenuSelectedFileExit(wxCommandEvent& event);

    void OnMenuSelectedViewGridDouble(wxCommandEvent& event);
    void OnMenuSelectedViewGridHalve(wxCommandEvent& event);
    void OnMenuSelectedViewGridCustom(wxCommandEvent& event);

    void OnButtonClickedSelectionPreview(wxCommandEvent& event);
    void OnTextChangeScriptParams(wxCommandEvent& event);

    void OnButtonClickedWorldScriptBrowse(wxCommandEvent& event);
    void OnButtonClickedWorldBackdropBrowse(wxCommandEvent& event);

    void        BuildWorldAliasList(wxArrayString* aliasList) const;
    void        SelectPaletteAlias(const char* alias);
    Vector2i    SnapPointToGrid(const Vector2i& position) const;

    DECLARE_EVENT_TABLE()

    boost::weak_ptr<Actor>      m_actorSelection;
    std::vector<ActorSummary>   m_actorSummary;
    std::vector<WorldSummary>   m_worldSummary;
    int                         m_worldGridSize;

    wxTextCtrl* m_textWorldScript;
    wxTextCtrl* m_textWorldBackdrop;

    wxListCtrl* m_listPalette;
    wxTextCtrl* m_textPaletteFilter;

    wxNotebook*         m_notebookSelection;
    wxPanel*            m_panelSelectionGeneral;
    wxScrolledWindow*   m_scrollWinSelectionScript;
    wxTextCtrl*         m_textSelectionName;
    wxTextCtrl*         m_textSelectionPositionX;
    wxTextCtrl*         m_textSelectionPositionY;
    wxBitmapButton*     m_buttonSelectionPreview;
};
