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
#include "FrameMapEditor.h"
#include "Surface.h"
#include "Actor.h"
#include "ActorPropertyScript.h"

BEGIN_EVENT_TABLE(FrameMapEditor, wxFrame)
    EVT_CLOSE(                                      FrameMapEditor::OnClose)
    EVT_SHOW(                                       FrameMapEditor::OnShow)
    EVT_MENU(ID_MENU_FILE_NEW,                      FrameMapEditor::OnMenuSelectedFileNew)
    EVT_MENU(ID_MENU_FILE_OPEN,                     FrameMapEditor::OnMenuSelectedFileOpen)
    EVT_MENU(ID_MENU_FILE_SAVE,                     FrameMapEditor::OnMenuSelectedFileSave)
    EVT_MENU(ID_MENU_FILE_SAVE_AS,                  FrameMapEditor::OnMenuSelectedFileSaveAs)
    EVT_MENU(ID_MENU_FILE_EXIT,                     FrameMapEditor::OnMenuSelectedFileExit)
    EVT_MENU(ID_MENU_VIEW_GRID_DOUBLE,              FrameMapEditor::OnMenuSelectedViewGridDouble)
    EVT_MENU(ID_MENU_VIEW_GRID_HALVE,               FrameMapEditor::OnMenuSelectedViewGridHalve)
    EVT_MENU(ID_MENU_VIEW_GRID_CUSTOM,              FrameMapEditor::OnMenuSelectedViewGridCustom)
    EVT_TEXT_ENTER(ID_WORLD_SCRIPT,                 FrameMapEditor::OnModifiedUiWorld)
    EVT_BUTTON(ID_WORLD_SCRIPT_BROWSE,              FrameMapEditor::OnButtonClickedWorldScriptBrowse)
    EVT_TEXT_ENTER(ID_WORLD_BACKDROP,               FrameMapEditor::OnModifiedUiWorld)
    EVT_BUTTON(ID_WORLD_BACKDROP_BROWSE,            FrameMapEditor::OnButtonClickedWorldBackdropBrowse)
    EVT_TEXT_ENTER(ID_SELECTION_NAME,               FrameMapEditor::OnModifiedUiSelection)
    EVT_TEXT_ENTER(ID_SELECTION_POSITION_X,         FrameMapEditor::OnModifiedUiSelection)
    EVT_TEXT_ENTER(ID_SELECTION_POSITION_Y,         FrameMapEditor::OnModifiedUiSelection)
    EVT_BUTTON(ID_SELECTION_PREVIEW,                FrameMapEditor::OnButtonClickedSelectionPreview)
    EVT_TEXT_ENTER(ID_SELECTION_PARAMS,             FrameMapEditor::OnTextChangeScriptParams)
    EVT_TEXT_ENTER(ID_PALETTE_FILTER,               FrameMapEditor::OnModifiedUiPalette)
END_EVENT_TABLE()

FrameMapEditor::FrameMapEditor(wxWindow* parent) :
        wxFrame(parent, wxID_ANY, wxT("Map Editor"), wxDefaultPosition, wxSize(500, 800), wxDEFAULT_FRAME_STYLE),
        m_worldGridSize(32),
        m_textWorldScript(NULL),
        m_textWorldBackdrop(NULL),
        m_listPalette(NULL),
        m_textPaletteFilter(NULL),
        m_notebookSelection(NULL),
        m_panelSelectionGeneral(NULL),
        m_scrollWinSelectionScript(NULL),
        m_textSelectionName(NULL),
        m_textSelectionPositionX(NULL),
        m_textSelectionPositionY(NULL),
        m_buttonSelectionPreview(NULL)
{
    wxBoxSizer* sizerMain = new wxBoxSizer(wxVERTICAL);
    sizerMain->Add(InitializeUiWorld(), 0, wxALL | wxEXPAND, 5);
    sizerMain->Add(InitializeUiPalette(), 1, wxALL | wxEXPAND, 5);
    sizerMain->Add(InitializeUiSelection(), 0, wxALL | wxEXPAND, 5);
    SetSizer(sizerMain);

    InitializeUiMenuBar();
}

void FrameMapEditor::InitializeUiMenuBar()
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_MENU_FILE_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(ID_MENU_FILE_OPEN, wxT("&Open\tCtrl+O"));
    menuFile->Append(ID_MENU_FILE_SAVE, wxT("&Save\tCtrl+S"));
    menuFile->Append(ID_MENU_FILE_SAVE_AS, wxT("Save &As...\tCtrl+Shift+S"));
    menuFile->AppendSeparator();
    menuFile->Append(ID_MENU_FILE_EXIT, wxT("E&xit\tCtrl+Q"));

    wxMenu* menuViewGrid = new wxMenu();
    menuViewGrid->Append(ID_MENU_VIEW_GRID_HALVE, wxT("&Halve\tCtrl+H"));
    menuViewGrid->Append(ID_MENU_VIEW_GRID_DOUBLE, wxT("&Double\tCtrl+D"));
    menuViewGrid->AppendSeparator();
    menuViewGrid->Append(ID_MENU_VIEW_GRID_CUSTOM, wxT("&Custom...\tCtrl+G"));

    wxMenu* menuView = new wxMenu();
    menuView->Append(-1, wxT("&Grid"), menuViewGrid);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuView, wxT("&View"));

    SetMenuBar(menuBar);
}

wxSizer* FrameMapEditor::InitializeUiWorld()
{
    wxBoxSizer* sizerWorldScript = new wxBoxSizer(wxHORIZONTAL);
    sizerWorldScript->Add(new wxStaticText(this, wxID_ANY, wxT("Script")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerWorldScript->Add(m_textWorldScript = new wxTextCtrl(this, ID_WORLD_SCRIPT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerWorldScript->Add(new wxButton(this, ID_WORLD_SCRIPT_BROWSE, wxT("&Browse...")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxBoxSizer* sizerWorldBackdrop = new wxBoxSizer(wxHORIZONTAL);
    sizerWorldBackdrop->Add(new wxStaticText(this, wxID_ANY, wxT("Backdrop")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerWorldBackdrop->Add(m_textWorldBackdrop = new wxTextCtrl(this, ID_WORLD_BACKDROP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerWorldBackdrop->Add(new wxButton(this, ID_WORLD_BACKDROP_BROWSE, wxT("&Browse...")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticBoxSizer* sizerWorld = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("World")), wxVERTICAL);
    sizerWorld->Add(sizerWorldScript, 0, wxEXPAND, 5);
    sizerWorld->Add(sizerWorldBackdrop, 0, wxEXPAND, 5);
    return sizerWorld;
}

wxSizer* FrameMapEditor::InitializeUiPalette()
{
    wxBoxSizer* sizerPaletteSettings = new wxBoxSizer(wxHORIZONTAL);
    sizerPaletteSettings->Add(new wxStaticText(this, wxID_ANY, wxT("Alias filter")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerPaletteSettings->Add(m_textPaletteFilter = new wxTextCtrl(this, ID_PALETTE_FILTER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticBoxSizer* sizerPalette = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Palette")), wxVERTICAL);
    m_listPalette = new wxListCtrl(this, ID_PALETTE_LIST, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_SINGLE_SEL);
    m_listPalette->SetForegroundColour(wxColour(0xff, 0xff, 0xff));
    m_listPalette->SetBackgroundColour(wxColour(0x00, 0x00, 0x00));
    sizerPalette->Add(m_listPalette, 1, wxALL | wxEXPAND, 5);
    sizerPalette->Add(sizerPaletteSettings, 0, wxEXPAND, 5);
    return sizerPalette;
}

wxSizer* FrameMapEditor::InitializeUiSelection()
{
    m_notebookSelection = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);

    m_panelSelectionGeneral = new wxPanel(m_notebookSelection, wxID_ANY);

    wxBoxSizer* sizerSelectionSettingsName = new wxBoxSizer(wxHORIZONTAL);
    sizerSelectionSettingsName->Add(new wxStaticText(m_panelSelectionGeneral, wxID_ANY, wxT("Name")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerSelectionSettingsName->Add(m_textSelectionName = new wxTextCtrl(m_panelSelectionGeneral, ID_SELECTION_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticBoxSizer* sizerSelectionSettingsPosition = new wxStaticBoxSizer(new wxStaticBox( m_panelSelectionGeneral, wxID_ANY, wxT("Position")), wxHORIZONTAL);
    sizerSelectionSettingsPosition->Add(new wxStaticText(m_panelSelectionGeneral, wxID_ANY, wxT("x")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerSelectionSettingsPosition->Add(m_textSelectionPositionX = new wxTextCtrl(m_panelSelectionGeneral, ID_SELECTION_POSITION_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerSelectionSettingsPosition->Add(new wxStaticText(m_panelSelectionGeneral, wxID_ANY, wxT("y")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerSelectionSettingsPosition->Add(m_textSelectionPositionY = new wxTextCtrl(m_panelSelectionGeneral, ID_SELECTION_POSITION_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxBoxSizer* sizerSelectionSettings = new wxBoxSizer(wxVERTICAL);
    sizerSelectionSettings->Add(sizerSelectionSettingsName, 0, wxEXPAND, 5);
    sizerSelectionSettings->Add(sizerSelectionSettingsPosition, 0, wxALL | wxEXPAND, 5);

    wxBoxSizer* sizerSelectionSettingsPreview = new wxBoxSizer(wxHORIZONTAL);
    sizerSelectionSettingsPreview->Add(sizerSelectionSettings, 1, wxEXPAND, 5);
    sizerSelectionSettingsPreview->Add(m_buttonSelectionPreview = new wxBitmapButton(m_panelSelectionGeneral, ID_SELECTION_PREVIEW, wxNullBitmap, wxDefaultPosition, wxSize(96, 96)), 0, wxALL, 5);

    m_scrollWinSelectionScript = new wxScrolledWindow(m_notebookSelection);
    m_scrollWinSelectionScript->SetScrollRate(5, 5);

    m_panelSelectionGeneral->SetSizer(sizerSelectionSettingsPreview);
    m_notebookSelection->AddPage(m_panelSelectionGeneral, wxT("General"), true);
    m_notebookSelection->AddPage(m_scrollWinSelectionScript, wxT("Script"), false);

    wxStaticBoxSizer* sizerSelection = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Selection")), wxVERTICAL);
    sizerSelection->Add(m_notebookSelection, 1, wxEXPAND | wxALL, 5);
    return sizerSelection;
}

void FrameMapEditor::UpdateUiWorld()
{
    m_textWorldBackdrop->SetValue(wxString::FromAscii(System::QueryWorldContext()->GetBackdropSpriteAlias()));
    m_textWorldScript->SetValue(wxString::FromAscii(System::QueryWorldContext()->GetScriptAlias()));
}

void FrameMapEditor::UpdateUiPalette()
{
    wxImage bitmapMissing = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE).ConvertToImage();

    Vector2i targetSize(bitmapMissing.GetWidth(), bitmapMissing.GetHeight());
    for (std::vector<ActorSummary>::iterator iter = m_actorSummary.begin(); iter != m_actorSummary.end(); ++iter)
    {
        if (iter->thumbnail)
        {
            targetSize.x = MAX(targetSize.x, iter->thumbnail->GetSize().x);
            targetSize.y = MAX(targetSize.y, iter->thumbnail->GetSize().y);
        }
    }

    std::vector<ActorSummary> actorSummaryFiltered;
    wxImageList* actorThumbnails = new wxImageList(targetSize.x, targetSize.y, true, m_actorSummary.size());
    const wxString actorAliasFilter = m_textPaletteFilter->GetValue();

    for (std::vector<ActorSummary>::iterator iter = m_actorSummary.begin(); iter != m_actorSummary.end(); ++iter)
    {
        const wxString actorAlias(wxString::FromAscii(iter->alias.c_str()));
        if (actorAliasFilter.IsEmpty() || actorAlias.Lower().Contains(actorAliasFilter.Lower()))
        {
            wxBitmap actorThumbnail = wxNullBitmap;
            if (iter->thumbnail)
            {
                iter->thumbnail->Lock();
                byte* const data = static_cast<byte*>(iter->thumbnail->GetData());
                wxImage image(iter->thumbnail->GetSize().x, iter->thumbnail->GetSize().y, data, true);
                image.Resize(wxSize(targetSize.x, targetSize.y), wxPoint());
                actorThumbnail = wxBitmap(image);
                iter->thumbnail->Unlock();
            }
            actorThumbnails->Add(actorThumbnail.IsOk() ? actorThumbnail : bitmapMissing);
            actorSummaryFiltered.push_back(*iter);
        }
    }

    m_listPalette->ClearAll();
    m_listPalette->AssignImageList(actorThumbnails, wxIMAGE_LIST_NORMAL);

    for (size_t i = 0; i < actorSummaryFiltered.size(); ++i)
    {
        m_listPalette->InsertItem(actorSummaryFiltered.size(), wxString::FromAscii(actorSummaryFiltered[i].alias.c_str()), i);
    }
}

void FrameMapEditor::UpdateUiSelection()
{
    boost::shared_ptr<Actor> actorSelection = m_actorSelection.lock();
    m_notebookSelection->Enable(actorSelection);
    m_scrollWinSelectionScript->DestroyChildren();
    if (!actorSelection)
    {
        return;
    }

    m_textSelectionName->SetValue(wxString::FromAscii(actorSelection->GetName()));
    m_textSelectionPositionX->SetValue(wxString::Format(wxT("%d"), actorSelection->GetPosition().x));
    m_textSelectionPositionY->SetValue(wxString::Format(wxT("%d"), actorSelection->GetPosition().y));

    boost::shared_ptr<ActorPropertyScript> propertyScript = actorSelection->GetProperty<ActorPropertyScript>().lock();
    if (propertyScript)
    {
        std::vector<std::string> scriptParameters;
        propertyScript->EnumerateParameters(&scriptParameters);

        wxBoxSizer* sizerScript = new wxBoxSizer(wxVERTICAL);
        for (size_t i = 0; i < scriptParameters.size(); ++i)
        {
            wxBoxSizer* sizerParameter = new wxBoxSizer(wxHORIZONTAL);
            const char* name = scriptParameters[i].c_str();
            wxStaticText* textLabel = new wxStaticText(m_scrollWinSelectionScript, wxID_ANY, wxString::FromAscii(name), wxDefaultPosition, wxSize(75,-1));
            sizerParameter->Add(textLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

            ScriptParameter parameter;
            if (propertyScript->GetParameter(name, &parameter))
            {
                wxTextCtrl* textCtrlValue = new wxTextCtrl(
                    m_scrollWinSelectionScript,
                    ID_SELECTION_PARAMS,
                    wxString::FromAscii(parameter.value.c_str()),
                    wxDefaultPosition,
                    wxDefaultSize,
                    wxTE_PROCESS_ENTER,
                    wxDefaultValidator,
                    wxString::FromAscii(name)
                );

                sizerParameter->Add(textCtrlValue, 1, wxALIGN_CENTER_VERTICAL, 5);
            }

            sizerScript->Add(sizerParameter, 0, wxEXPAND, 5);
        }

        m_scrollWinSelectionScript->SetSizer(sizerScript);
        m_scrollWinSelectionScript->Layout();
    }

    wxBitmap actorThumbnail = wxNullBitmap;
    for (std::vector<ActorSummary>::iterator iter = m_actorSummary.begin(); iter != m_actorSummary.end(); ++iter)
    {
        if (iter->thumbnail && strcmp(actorSelection->GetAlias(), iter->alias.c_str()) == 0)
        {
            iter->thumbnail->Lock();
            actorThumbnail = wxBitmap(wxImage(iter->thumbnail->GetSize().x, iter->thumbnail->GetSize().y, static_cast<byte*>(iter->thumbnail->GetData()), true));
            iter->thumbnail->Unlock();
        }
    }
    m_buttonSelectionPreview->SetBitmapLabel(actorThumbnail);
}

void FrameMapEditor::OnModifiedUiWorld(wxCommandEvent& event)
{
    System::QueryWorldContext()->SetBackdropSpriteAlias(m_textWorldBackdrop->GetValue().ToAscii());
    System::QueryWorldContext()->SetScriptAlias(m_textWorldScript->GetValue().ToAscii());
}

void FrameMapEditor::OnModifiedUiPalette(wxCommandEvent& event)
{
    UpdateUiPalette();
}

void FrameMapEditor::OnModifiedUiSelection(wxCommandEvent& event)
{
    boost::shared_ptr<Actor> actorSelection = m_actorSelection.lock();
    if (!actorSelection)
    {
        return;
    }

    long actorPositionX = actorSelection->GetPosition().x;
    m_textSelectionPositionX->GetValue().ToLong(&actorPositionX);

    long actorPositionY = actorSelection->GetPosition().y;
    m_textSelectionPositionY->GetValue().ToLong(&actorPositionY);

    actorSelection->SetName(m_textSelectionName->GetValue().ToAscii());
    actorSelection->SetPosition(Vector2i(actorPositionX, actorPositionY));

    UpdateUiSelection();
}

void FrameMapEditor::OnWorldMouseMove(const InputMouse& args)
{
    boost::shared_ptr<Actor> actorSelection = m_actorSelection.lock();
    if (actorSelection && args.buttonState == SDL_BUTTON_LMASK)
    {
        actorSelection->SetPosition(SnapPointToGrid(args.position));
        UpdateUiSelection();
    }

    if (args.buttonState == SDL_BUTTON_RMASK)
    {
        const Vector2i cameraPosition = System::QueryWorldContext()->GetCameraTarget();
        System::QueryWorldContext()->SetCameraTarget(Vector2i(cameraPosition.x - args.delta.x, cameraPosition.y - args.delta.y));
    }
}

void FrameMapEditor::OnWorldMouseButtonDown(const InputMouse& args)
{
    boost::shared_ptr<Actor> actorSelection = System::QueryWorldContext()->PickActor(args.position).lock();

    if (args.button == SDL_BUTTON_LEFT)
    {
        const int index = m_listPalette->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

        if (args.modifier == KMOD_SHIFT && index != -1)
        {
            const std::string actorAlias(m_listPalette->GetItemText(index).ToAscii());

            ActorSummary summary;
            if (!actorSelection || (System::SummarizeActor(actorAlias.c_str(), &summary) && actorSelection->GetLayer() < summary.layer))
            {
                actorSelection = System::QueryWorldContext()->AddActor(actorAlias.c_str(), SnapPointToGrid(args.position)).lock();
            }
        }
        if (args.modifier == KMOD_CTRL && actorSelection)
        {
            System::QueryWorldContext()->RemoveActor(actorSelection->GetId());
            actorSelection.reset();
        }

        m_actorSelection = actorSelection;
        UpdateUiSelection();
    }
    else if (args.button == SDL_BUTTON_RIGHT && actorSelection)
    {
        SelectPaletteAlias(actorSelection->GetAlias());
    }
}

void FrameMapEditor::OnClose(wxCloseEvent&)
{
    Hide();
}

void FrameMapEditor::OnShow(wxShowEvent& event)
{
    if (event.GetShow())
    {
        if (m_actorSummary.size() == 0)
        {
            System::SummarizeActors(&m_actorSummary);
        }

        if (m_worldSummary.size() == 0)
        {
            System::SummarizeWorlds(&m_worldSummary);
        }

        System::QueryWorldContext()->AddListener(this);

        UpdateUiWorld();
        UpdateUiPalette();
        UpdateUiSelection();
    }
    else if (System::QueryWorldContext() != NULL)
    {
        System::QueryWorldContext()->RemoveListener(this);
    }
}

void FrameMapEditor::SelectPaletteAlias(const char* alias)
{
    const int index = m_listPalette->FindItem(-1, wxString::FromAscii(alias));

    if (index != -1)
    {
        const int listFlags = wxLIST_STATE_SELECTED;
        m_listPalette->SetItemState(index, listFlags, listFlags);
        m_listPalette->EnsureVisible(index);
    }
    else if (m_listPalette->GetItemCount() != static_cast<int>(m_actorSummary.size()))
    {
        m_textPaletteFilter->Clear();
        UpdateUiPalette();
        SelectPaletteAlias(alias);
    }
}

Vector2i FrameMapEditor::SnapPointToGrid(const Vector2i& position) const
{
    Vector2i gridPosition(position.x / m_worldGridSize, position.y / m_worldGridSize);

    if (position.x < 0)
    {
        --gridPosition.x;
    }
    if (position.y < 0)
    {
        --gridPosition.y;
    }

    return Vector2i(gridPosition.x * m_worldGridSize, gridPosition.y * m_worldGridSize);
}

void FrameMapEditor::BuildWorldAliasList(wxArrayString* aliasList) const
{
    for (std::vector<WorldSummary>::const_iterator iter = m_worldSummary.begin(); iter != m_worldSummary.end(); ++iter)
    {
        aliasList->Add(wxString::FromAscii(iter->alias.c_str()));
    }
}

void FrameMapEditor::OnMenuSelectedFileNew(wxCommandEvent& event)
{
    System::QueryWorldContext()->ClearActors();
}

void FrameMapEditor::OnMenuSelectedFileOpen(wxCommandEvent& event)
{
    wxArrayString worldNames;
    BuildWorldAliasList(&worldNames);

    wxSingleChoiceDialog dialog(this, wxT("Select a world to load"), wxT("Load world"), worldNames);
    if (dialog.ShowModal() == wxID_OK)
    {
        System::QueryWorldContext()->LoadWorld(dialog.GetStringSelection().ToAscii());
        UpdateUiWorld();
        UpdateUiSelection();
    }
}

void FrameMapEditor::OnMenuSelectedFileSave(wxCommandEvent& event)
{
    const char* worldAlias = System::QueryWorldContext()->GetWorldAlias();

    if (worldAlias == NULL)
    {
        OnMenuSelectedFileSaveAs(event);
    }
    else
    {
        System::QueryWorldContext()->SaveWorld(worldAlias);
    }
}

void FrameMapEditor::OnMenuSelectedFileSaveAs(wxCommandEvent& event)
{
    wxArrayString worldNames;
    BuildWorldAliasList(&worldNames);

    wxSingleChoiceDialog dialog(this, wxT("Select a world to save"), wxT("Save world"), worldNames);
    if (dialog.ShowModal() == wxID_OK)
    {
        System::QueryWorldContext()->SaveWorld(dialog.GetStringSelection().ToAscii());
    }
}

void FrameMapEditor::OnMenuSelectedFileExit(wxCommandEvent& event)
{
    Hide();
}

void FrameMapEditor::OnMenuSelectedViewGridHalve(wxCommandEvent& event)
{
    if (m_worldGridSize >= 2)
    {
        m_worldGridSize >>= 1;
    }
}

void FrameMapEditor::OnMenuSelectedViewGridDouble(wxCommandEvent& event)
{
    if (m_worldGridSize <= 1024)
    {
        m_worldGridSize <<= 1;
    }
}

void FrameMapEditor::OnMenuSelectedViewGridCustom(wxCommandEvent& event)
{
    wxTextEntryDialog dialog(this, wxT("Specify a world grid size"), wxT("World grid size"), wxString::Format(wxT("%d"), m_worldGridSize));
    if (dialog.ShowModal() != wxID_OK)
    {
        return;
    }

    long worldGridSize = m_worldGridSize;
    if (dialog.GetValue().ToLong(&worldGridSize) && worldGridSize >= 1 && worldGridSize <= 1024)
    {
        m_worldGridSize = worldGridSize;
    }
}

void FrameMapEditor::OnButtonClickedSelectionPreview(wxCommandEvent& event)
{
    const boost::shared_ptr<Actor> actorSelection = m_actorSelection.lock();
    if (actorSelection)
    {
        SelectPaletteAlias(actorSelection->GetAlias());
    }
}

void FrameMapEditor::OnTextChangeScriptParams(wxCommandEvent& event)
{
    boost::shared_ptr<Actor> actorSelection = m_actorSelection.lock();
    if (!actorSelection)
    {
        return;
    }

    boost::shared_ptr<ActorPropertyScript> propertyScript = actorSelection->GetProperty<ActorPropertyScript>().lock();
    if (!propertyScript)
    {
        return;
    }

    wxTextCtrl* selectedTextCtrl = static_cast<wxTextCtrl*>(m_scrollWinSelectionScript->FindFocus());
    const std::string name(selectedTextCtrl->GetName().ToAscii());
    const std::string value(selectedTextCtrl->GetValue().ToAscii());

    ScriptParameter parameter;
    if (!propertyScript->GetParameter(name.c_str(), &parameter))
    {
        return;
    }

    propertyScript->SetParameter(name.c_str(), ScriptParameter(value.c_str(), parameter.type));
    propertyScript->ResetScriptState();
}

void FrameMapEditor::OnButtonClickedWorldScriptBrowse(wxCommandEvent& event)
{
    std::vector<ScriptSummary> summary;
    System::SummarizeScripts(&summary);

    wxArrayString scriptNames;
    for (std::vector<ScriptSummary>::iterator iter = summary.begin(); iter != summary.end(); ++iter)
    {
        scriptNames.Add(wxString::FromAscii(iter->alias.c_str()));
    }

    wxSingleChoiceDialog dialog(this, wxT("Select a script"), wxT("Load script"), scriptNames);
    if (dialog.ShowModal() == wxID_OK)
    {
        System::QueryWorldContext()->SetScriptAlias(dialog.GetStringSelection().ToAscii());
        UpdateUiWorld();
    }
}

void FrameMapEditor::OnButtonClickedWorldBackdropBrowse(wxCommandEvent& event)
{
    std::vector<SpriteSummary> summary;
    System::SummarizeSprites(&summary);

    wxArrayString spriteNames;
    for (std::vector<SpriteSummary>::iterator iter = summary.begin(); iter != summary.end(); ++iter)
    {
        spriteNames.Add(wxString::FromAscii(iter->alias.c_str()));
    }

    wxSingleChoiceDialog dialog(this, wxT("Select a sprite"), wxT("Load sprite"), spriteNames);
    if (dialog.ShowModal() == wxID_OK)
    {
        System::QueryWorldContext()->SetBackdropSpriteAlias(dialog.GetStringSelection().ToAscii());
        UpdateUiWorld();
    }
}
