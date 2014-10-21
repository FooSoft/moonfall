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
#include "FrameSpriteEditor.h"

BEGIN_EVENT_TABLE(FrameSpriteEditor, wxFrame)
    EVT_CLOSE(                      FrameSpriteEditor::OnClose)
    EVT_MENU(ID_MENU_FILE_NEW,      FrameSpriteEditor::OnMenuSelectedFileNew)
    EVT_MENU(ID_MENU_FILE_OPEN,     FrameSpriteEditor::OnMenuSelectedFileOpen)
    EVT_MENU(ID_MENU_FILE_SAVE,     FrameSpriteEditor::OnMenuSelectedFileSave)
    EVT_MENU(ID_MENU_FILE_SAVE_AS,  FrameSpriteEditor::OnMenuSelectedFileSaveAs)
    EVT_MENU(ID_MENU_FILE_EXIT,     FrameSpriteEditor::OnMenuSelectedFileExit)
    EVT_MENU(ID_MENU_IMAGE_INSERT,  FrameSpriteEditor::OnMenuSelectedImageInsert)
    EVT_MENU(ID_MENU_IMAGE_DELETE,  FrameSpriteEditor::OnMenuSelectedImageDelete)
    EVT_MENU(ID_MENU_SPRITE_RENAME, FrameSpriteEditor::OnMenuSelectedSpriteRename)
    EVT_MENU(ID_MENU_SPRITE_DELETE, FrameSpriteEditor::OnMenuSelectedSpriteDelete)
    EVT_MENU(ID_MENU_SHAPE_RENAME,  FrameSpriteEditor::OnMenuSelectedShapeRename)
    EVT_MENU(ID_MENU_SHAPE_DELETE,  FrameSpriteEditor::OnMenuSelectedShapeDelete)
END_EVENT_TABLE()

FrameSpriteEditor::FrameSpriteEditor(wxWindow* parent, const wxString& manifestFilename) :
        wxFrame(parent, wxID_ANY, wxT("Sprite Editor"), wxDefaultPosition, wxSize(800, 600)),
        m_listImages(NULL),
        m_listSprites(NULL),
        m_listShapes(NULL),
        m_scrolledWindow(NULL),
        m_panelCanvas(NULL),
        m_textGrid(NULL),
        m_draggingSprite(false),
        m_draggingShape(false),
        m_gridSize(16),
        m_imageIndex(wxNOT_FOUND),
        m_spriteIndex(wxNOT_FOUND),
        m_shapeIndex(wxNOT_FOUND)
{
    InitializeUi();
    InitializeMenuBar();

    m_currentFilename = manifestFilename;

    if (!manifestFilename.IsEmpty() && LoadManifest(manifestFilename))
    {
        UpdateImageList();
        UpdateSpriteList();
        UpdateShapeList();
        UpdateCanvas();
    }
}

bool FrameSpriteEditor::LoadManifest(const wxString& filename)
{
    ClearManifest();

    if (!m_spriteManager.LoadManifest(filename.ToAscii().data()))
    {
        return false;
    }

    std::vector<SpriteSummary> summary;
    m_spriteManager.SummarizeSprites(&summary);

    for (std::vector<SpriteSummary>::iterator summaryIter = summary.begin(); summaryIter != summary.end(); ++summaryIter)
    {
        std::vector<Image>::iterator imageIter;
        for (imageIter = m_images.begin(); imageIter != m_images.end(); ++imageIter)
        {
            if (imageIter->filename == summaryIter->asset)
            {
                break;
            }
        }

        if (imageIter == m_images.end())
        {
            m_images.push_back(Image(summaryIter->asset));
            imageIter = m_images.begin() + m_images.size() - 1;
        }

        imageIter->sprites.push_back(Sprite(summaryIter->alias, summaryIter->position, summaryIter->size, summaryIter->shapes));
    }

    return true;
}

bool FrameSpriteEditor::SaveManifest(const wxString& filename)
{
    m_spriteManager.ClearManifest();

    for (std::vector<Image>::iterator imageIter = m_images.begin(); imageIter != m_images.end(); ++imageIter)
    {
        for (std::vector<Sprite>::iterator spriteIter = imageIter->sprites.begin(); spriteIter != imageIter->sprites.end(); ++spriteIter)
        {
            m_spriteManager.AppendManifest(
                spriteIter->alias.c_str(),
                imageIter->filename.c_str(),
                spriteIter->position,
                spriteIter->size,
                spriteIter->shapes
            );
        }
    }

    return m_spriteManager.SaveManifest(filename.ToAscii().data());
}

void FrameSpriteEditor::ClearManifest()
{
    m_spriteManager.ClearManifest();
    m_images.clear();
}

void FrameSpriteEditor::OnMenuSelectedFileNew(wxCommandEvent& event)
{
    m_currentFilename.Empty();
    ClearManifest();

    UpdateImageList();
    UpdateSpriteList();
    UpdateShapeList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnMenuSelectedFileOpen(wxCommandEvent& event)
{
    const wxString filename = wxFileSelector(
                                  wxT("Choose a sprite manifest file"),
                                  wxEmptyString,
                                  wxEmptyString,
                                  wxEmptyString,
                                  wxT("XML files (*.xml)|*.xml"),
                                  wxFD_OPEN,
                                  this
                              );

    if (filename.IsEmpty())
    {
        return;
    }

    m_currentFilename = filename;
    LoadManifest(filename);

    UpdateImageList();
    UpdateSpriteList();
    UpdateShapeList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnMenuSelectedFileSave(wxCommandEvent& event)
{
    if (m_currentFilename.IsEmpty())
    {
        OnMenuSelectedFileSaveAs(event);
    }
    else
    {
        SaveManifest(m_currentFilename);
    }
}

void FrameSpriteEditor::OnMenuSelectedFileSaveAs(wxCommandEvent& event)
{
    const wxString filename = wxFileSelector(
                                  wxT("Choose a sprite manifest file"),
                                  wxEmptyString,
                                  wxEmptyString,
                                  wxEmptyString,
                                  wxT("XML files (*.xml)|*.xml"),
                                  wxFD_SAVE,
                                  this
                              );

    if (filename.IsEmpty())
    {
        return;
    }

    m_currentFilename = filename;
    SaveManifest(filename);
}

void FrameSpriteEditor::OnMenuSelectedFileExit(wxCommandEvent& event)
{
    Hide();
}

void FrameSpriteEditor::OnCanvasLeftDown(wxMouseEvent& event)
{
    if (m_imageIndex != wxNOT_FOUND)
    {
        if (!event.ControlDown())
        {
            m_draggingSprite = true;
        }
        else if (m_spriteIndex != wxNOT_FOUND)
        {
            m_draggingShape = true;
        }
    }

    m_marquee = Marquee(AdjustPosition(Vector2i(event.GetX(), event.GetY())), Vector2i(0, 0));
    UpdateCanvas();
}

void FrameSpriteEditor::OnCanvasLeftUp(wxMouseEvent& event)
{
    do
    {
        if (m_imageIndex == wxNOT_FOUND || m_marquee.size.x == 0 || m_marquee.size.y == 0)
        {
            break;
        }

        Image& image = m_images[m_imageIndex];

        if (m_draggingSprite)
        {
            wxTextEntryDialog dialog(this, wxT("Sprite alias"), wxT("Input a sprite alias"));
            if (dialog.ShowModal() != wxID_OK || dialog.GetValue().IsEmpty())
            {
                break;
            }

            image.sprites.push_back(Sprite(dialog.GetValue().ToAscii().data(), m_marquee.position, m_marquee.size));
            UpdateSpriteList();
        }
        else if (m_draggingShape && m_spriteIndex != wxNOT_FOUND)
        {
            Sprite& sprite = image.sprites[m_spriteIndex];

            long shapeType = 0;
            wxTextEntryDialog dialogId(this, wxT("Shape type"), wxT("Input shape type"));
            if (dialogId.ShowModal() != wxID_OK || !dialogId.GetValue().ToLong(&shapeType) || shapeType >= ACTOR_SHAPE_TYPE_COUNT)
            {
                break;
            }

            long shapeData = 0;
            wxTextEntryDialog dialogData(this, wxT("Shape data"), wxT("Input shape data"));
            if (dialogData.ShowModal() != wxID_OK || !dialogData.GetValue().ToLong(&shapeData))
            {
                break;
            }

            ActorShape& shape = sprite.shapes[shapeType];
            shape.data = shapeData;
            shape.position = m_marquee.position - sprite.position;
            shape.size = m_marquee.size;
            shape.enabled = true;;

            UpdateShapeList();
        }
    }
    while (false);

    m_draggingSprite = false;
    m_draggingShape = false;

    UpdateCanvas();
}

void FrameSpriteEditor::OnCanvasRightDown(wxMouseEvent& event)
{
    if (m_draggingSprite || m_draggingShape || m_imageIndex == wxNOT_FOUND)
    {
        return;
    }

    const Image& image = m_images[m_imageIndex];

    for (size_t i = 0; i < image.sprites.size(); ++i)
    {
        const Sprite& sprite = image.sprites[i];
        const Rect4i spriteRect(sprite.position.x, sprite.position.y, sprite.position.x + sprite.size.x, sprite.position.y + sprite.size.y);

        if (spriteRect.Contains(event.GetX(), event.GetY()))
        {
            m_spriteIndex = static_cast<int>(i);
            m_listSprites->SetSelection(m_spriteIndex);
            UpdateShapeList();
            UpdateCanvas();
            break;
        }
    }
}

void FrameSpriteEditor::OnCanvasMotion(wxMouseEvent& event)
{
    if (!m_draggingSprite && !m_draggingShape)
    {
        return;
    }

    Vector2i position = AdjustPosition(Vector2i(event.GetX(), event.GetY()));
    position.x = MAX(position.x, m_marquee.position.x);
    position.y = MAX(position.y, m_marquee.position.y);

    m_marquee.size = position - m_marquee.position;
    UpdateCanvas();
}

void FrameSpriteEditor::OnCanvasPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(m_panelCanvas);
    dc.SetBackground(wxBrush(wxColor(0x00, 0x00, 0x00), wxSOLID));
    dc.Clear();

    if (m_currentBitmap.IsOk())
    {
        dc.SetBrush(wxBrush(wxColor(0x00, 0x00, 0xff), wxSOLID));
        dc.SetPen(wxPen(wxColor(0x00, 0x00, 0x00), 1, wxTRANSPARENT));
        dc.DrawRectangle(BITMAP_OFFSET, BITMAP_OFFSET, m_currentBitmap.GetWidth(), m_currentBitmap.GetHeight());
        dc.DrawBitmap(m_currentBitmap, BITMAP_OFFSET, BITMAP_OFFSET, true);
    }

    dc.SetBrush(wxBrush(wxColor(0x00, 0x00, 0x00), wxTRANSPARENT));

    if (m_draggingSprite)
    {
        dc.SetPen(wxPen(wxColor(0x00, 0xff, 0x00), 2, wxSOLID));
        dc.DrawRectangle(m_marquee.position.x + BITMAP_OFFSET, m_marquee.position.y + BITMAP_OFFSET, m_marquee.size.x, m_marquee.size.y);
    }
    else if (m_draggingShape)
    {
        dc.SetPen(wxPen(wxColor(0xff, 0xff, 0x00), 2, wxSOLID));
        dc.DrawRectangle(m_marquee.position.x + BITMAP_OFFSET, m_marquee.position.y + BITMAP_OFFSET, m_marquee.size.x, m_marquee.size.y);
    }
    else if (m_spriteIndex != wxNOT_FOUND && m_imageIndex != wxNOT_FOUND)
    {
        const Sprite& sprite = m_images[m_imageIndex].sprites[m_spriteIndex];
        dc.SetPen(wxPen(wxColor(0x00, 0xff, 0x00), 2, wxSHORT_DASH));
        dc.DrawRectangle(sprite.position.x + BITMAP_OFFSET, sprite.position.y + BITMAP_OFFSET, sprite.size.x, sprite.size.y);

        if (m_shapeIndex != wxNOT_FOUND)
        {
            const ActorShape& shape = sprite.shapes[m_shapeIndex];
            if (shape.enabled)
            {
                dc.SetPen(wxPen(wxColor(0xff, 0xff, 0x00), 2, wxSHORT_DASH));
                dc.DrawRectangle(shape.position.x + sprite.position.x + BITMAP_OFFSET, shape.position.y + sprite.position.y + BITMAP_OFFSET, shape.size.x, shape.size.y);
            }
        }
    }
}

void FrameSpriteEditor::OnMenuSelectedImageInsert(wxCommandEvent& event)
{
    wxFileName filename = wxFileSelector(
                              wxT("Choose an image file"),
                              wxEmptyString,
                              wxEmptyString,
                              wxEmptyString,
                              wxT("PNG files (*.png)|*.png"),
                              wxFD_OPEN,
                              this
                          );

    if (!filename.IsOk() || !filename.MakeRelativeTo(wxGetCwd()))
    {
        return;
    }

    m_images.push_back(Image(filename.GetFullPath().ToAscii().data()));

    UpdateImageList();
    UpdateSpriteList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnMenuSelectedImageDelete(wxCommandEvent& event)
{
    if (m_imageIndex == wxNOT_FOUND)
    {
        return;
    }

    m_images.erase(m_images.begin() + m_imageIndex);

    UpdateImageList();
    UpdateSpriteList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnMenuSelectedSpriteRename(wxCommandEvent& event)
{
    if (m_imageIndex == wxNOT_FOUND || m_spriteIndex == wxNOT_FOUND)
    {
        return;
    }

    wxTextEntryDialog dialog(this, wxT("Sprite alias"), wxT("Input a sprite alias"), wxString::FromAscii(m_images[m_imageIndex].sprites[m_spriteIndex].alias.c_str()));
    if (dialog.ShowModal() != wxID_OK || dialog.GetValue().IsEmpty())
    {
        return;
    }

    m_images[m_imageIndex].sprites[m_spriteIndex].alias = dialog.GetValue().ToAscii().data();
    UpdateSpriteList();
}

void FrameSpriteEditor::OnMenuSelectedSpriteDelete(wxCommandEvent& event)
{
    if (m_imageIndex == wxNOT_FOUND || m_spriteIndex == wxNOT_FOUND)
    {
        return;
    }

    std::vector<Sprite>& sprites = m_images[m_imageIndex].sprites;
    sprites.erase(sprites.begin() + m_spriteIndex);
    UpdateSpriteList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnMenuSelectedShapeRename(wxCommandEvent& event)
{
}

void FrameSpriteEditor::OnMenuSelectedShapeDelete(wxCommandEvent& event)
{
    if (m_imageIndex == wxNOT_FOUND || m_spriteIndex == wxNOT_FOUND || m_shapeIndex == wxNOT_FOUND)
    {
        return;
    }

    m_images[m_imageIndex].sprites[m_spriteIndex].shapes[m_shapeIndex].enabled = false;

    UpdateShapeList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnImagesRightDown(wxMouseEvent& event)
{
    wxMenu menu;
    menu.Append(ID_MENU_IMAGE_INSERT, wxT("&Insert image..."));
    menu.Append(ID_MENU_IMAGE_DELETE, wxT("&Delete image"))->Enable(m_imageIndex != wxNOT_FOUND);
    PopupMenu(&menu);
}

void FrameSpriteEditor::OnSpritesRightDown(wxMouseEvent& event)
{
    wxMenu menu;
    menu.Append(ID_MENU_SPRITE_RENAME, wxT("&Rename sprite..."))->Enable(m_spriteIndex != wxNOT_FOUND);
    menu.Append(ID_MENU_SPRITE_DELETE, wxT("&Delete sprite"))->Enable(m_spriteIndex != wxNOT_FOUND);
    PopupMenu(&menu);
}

void FrameSpriteEditor::OnShapesRightDown(wxMouseEvent& event)
{
    wxMenu menu;
    menu.Append(ID_MENU_SHAPE_RENAME, wxT("&Rename shape..."))->Enable(m_shapeIndex != wxNOT_FOUND);
    menu.Append(ID_MENU_SHAPE_DELETE, wxT("&Delete shape"))->Enable(m_shapeIndex != wxNOT_FOUND);
    PopupMenu(&menu);
}

void FrameSpriteEditor::OnImagesSelected(wxCommandEvent& event)
{
    m_imageIndex = m_listImages->GetSelection();

    if (m_imageIndex == wxNOT_FOUND)
    {
        m_currentBitmap = wxNullBitmap;
    }
    else
    {
        const wxString filename = m_listImages->GetString(m_imageIndex);
        m_currentBitmap.LoadFile(filename, wxBITMAP_TYPE_PNG);
    }

    UpdateSpriteList();
    UpdateShapeList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnSpritesSelected(wxCommandEvent& event)
{
    m_spriteIndex = m_listSprites->GetSelection();
    UpdateShapeList();
    UpdateCanvas();
}

void FrameSpriteEditor::OnShapesSelected(wxCommandEvent& event)
{
    m_shapeIndex = m_listShapes->GetSelection();
    UpdateCanvas();
}

void FrameSpriteEditor::OnModifiedGrid(wxCommandEvent& event)
{
    long gridSize = m_gridSize;
    m_textGrid->GetValue().ToLong(&gridSize);
    m_gridSize = gridSize;
}

void FrameSpriteEditor::OnClose(wxCloseEvent& event)
{
    Hide();
}

void FrameSpriteEditor::UpdateCanvas()
{
    if (m_currentBitmap.IsOk())
    {
        m_panelCanvas->SetMinSize(wxSize(m_currentBitmap.GetWidth() + BITMAP_OFFSET, m_currentBitmap.GetHeight() + BITMAP_OFFSET));
    }
    else
    {
        m_panelCanvas->SetMinSize(wxSize(0, 0));
    }

    m_scrolledWindow->FitInside();
    m_panelCanvas->Refresh(false);
}

void FrameSpriteEditor::UpdateImageList()
{
    m_listImages->Clear();

    for (std::vector<Image>::iterator iter = m_images.begin(); iter != m_images.end(); ++iter)
    {
        m_listImages->Append(wxString::FromAscii(iter->filename.c_str()));
    }

    GetSizer()->Layout();
}

void FrameSpriteEditor::UpdateSpriteList()
{
    m_listSprites->Clear();

    if (m_imageIndex != wxNOT_FOUND)
    {
        std::vector<Sprite>& sprites = m_images[m_imageIndex].sprites;
        for (std::vector<Sprite>::iterator iter = sprites.begin(); iter != sprites.end(); ++iter)
        {
            m_listSprites->Append(wxString::FromAscii(iter->alias.c_str()));
        }
    }

    GetSizer()->Layout();
}

void FrameSpriteEditor::UpdateShapeList()
{
    static const wchar_t* shapeNames[] = { L"COLLISION_SOLID", L"COLLISION_INTERACT" };

    m_listShapes->Clear();

    if (m_imageIndex != wxNOT_FOUND && m_spriteIndex != wxNOT_FOUND)
    {
        const ActorShape* const shapes = m_images[m_imageIndex].sprites[m_spriteIndex].shapes;
        for (int i = 0; i < ACTOR_SHAPE_TYPE_COUNT; ++i)
        {
            if (shapes[i].enabled)
            {
                m_listShapes->Append(wxString::Format(wxT("%s: %d"), shapeNames[i], shapes[i].data));
            }
            else
            {
                m_listShapes->Append(wxString::Format(wxT("%s: NULL"), shapeNames[i]));
            }
        }
    }

    GetSizer()->Layout();
}

void FrameSpriteEditor::InitializeUi()
{
    m_listImages = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    m_listImages->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(FrameSpriteEditor::OnImagesRightDown), NULL, this);
    m_listImages->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(FrameSpriteEditor::OnImagesSelected), NULL, this);

    m_listSprites = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    m_listSprites->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(FrameSpriteEditor::OnSpritesRightDown), NULL, this);
    m_listSprites->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(FrameSpriteEditor::OnSpritesSelected), NULL, this);

    m_listShapes = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    m_listShapes->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(FrameSpriteEditor::OnShapesRightDown), NULL, this);
    m_listShapes->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(FrameSpriteEditor::OnShapesSelected), NULL, this);

    m_scrolledWindow = new wxScrolledWindow(this);
    m_scrolledWindow->SetScrollRate(1, 1);

    m_panelCanvas = new wxPanel(m_scrolledWindow);
    m_panelCanvas->SetBackgroundColour(wxColour(0x00, 0x00, 0x00));
    m_panelCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(FrameSpriteEditor::OnCanvasLeftDown), NULL, this);
    m_panelCanvas->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(FrameSpriteEditor::OnCanvasLeftUp), NULL, this);
    m_panelCanvas->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(FrameSpriteEditor::OnCanvasRightDown), NULL, this);
    m_panelCanvas->Connect(wxEVT_MOTION, wxMouseEventHandler(FrameSpriteEditor::OnCanvasMotion), NULL, this);
    m_panelCanvas->Connect(wxEVT_PAINT, wxPaintEventHandler(FrameSpriteEditor::OnCanvasPaint), NULL, this);

    m_textGrid = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_textGrid->Connect(wxEVT_COMMAND_TEXT_ENTER, wxTextEventHandler(FrameSpriteEditor::OnModifiedGrid), NULL, this);
    m_textGrid->SetValue(wxString::Format(wxT("%d"), m_gridSize));

    wxBoxSizer* sizerScroll = new wxBoxSizer(wxVERTICAL);
    sizerScroll->Add(m_panelCanvas, 1, wxEXPAND);
    m_scrolledWindow->SetSizer(sizerScroll);

    wxBoxSizer* sizerOptions = new wxBoxSizer(wxHORIZONTAL);
    sizerOptions->Add(new wxStaticText(this, wxID_ANY, wxT("Grid")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizerOptions->Add(m_textGrid, 0, wxALL, 5);

    wxBoxSizer* sizerAssets = new wxBoxSizer(wxVERTICAL);
    sizerAssets->Add(new wxStaticText(this, wxID_ANY, wxT("Images")), 0, wxALL, 5);
    sizerAssets->Add(m_listImages, 1, wxALL | wxEXPAND, 5);
    sizerAssets->Add(new wxStaticText(this, wxID_ANY, wxT("Sprites")), 0, wxALL, 5);
    sizerAssets->Add(m_listSprites, 1, wxALL | wxEXPAND, 5);
    sizerAssets->Add(new wxStaticText(this, wxID_ANY, wxT("Shapes")), 0, wxALL, 5);
    sizerAssets->Add(m_listShapes, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* sizerCanvas = new wxBoxSizer(wxVERTICAL);
    sizerCanvas->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);
    sizerCanvas->Add(sizerOptions, 0, wxALIGN_RIGHT, 5);

    wxBoxSizer* sizerLayout = new wxBoxSizer(wxHORIZONTAL);
    sizerLayout->Add(sizerAssets, 0, wxEXPAND);
    sizerLayout->Add(sizerCanvas, 1, wxEXPAND);
    SetSizer(sizerLayout);
}

void FrameSpriteEditor::InitializeMenuBar()
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_MENU_FILE_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(ID_MENU_FILE_OPEN, wxT("&Open\tCtrl+O"));
    menuFile->Append(ID_MENU_FILE_SAVE, wxT("&Save\tCtrl+S"));
    menuFile->Append(ID_MENU_FILE_SAVE_AS, wxT("Save &As...\tCtrl+Shift+S"));
    menuFile->AppendSeparator();
    menuFile->Append(ID_MENU_FILE_EXIT, wxT("E&xit\tCtrl+Q"));

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    SetMenuBar(menuBar);
}

Vector2i FrameSpriteEditor::AdjustPosition(const Vector2i& position) const
{
    const Vector2i mod(position.x % m_gridSize, position.y % m_gridSize);
    Vector2i positionAdj = position - mod;
    const Vector2i sign(positionAdj.x >= 0 ? 1 : -1, positionAdj.y >= 0 ? 1 : -1);

    if (abs(mod.x) > m_gridSize / 2)
    {
        positionAdj.x += m_gridSize * sign.x;
    }

    if (abs(mod.y) > m_gridSize / 2)
    {
        positionAdj.y += m_gridSize * sign.y;
    }

    return positionAdj - Vector2i(BITMAP_OFFSET, BITMAP_OFFSET);
}
