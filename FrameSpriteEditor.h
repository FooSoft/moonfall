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
#include "SpriteManifest.h"

class FrameSpriteEditor : public wxFrame
{
public:
    FrameSpriteEditor(wxWindow* parent, const wxString& manifestFilename = wxEmptyString);

private:
    enum
    {
        BITMAP_OFFSET = 64
    };

    enum
    {
        ID_MENU_FILE_NEW,
        ID_MENU_FILE_OPEN,
        ID_MENU_FILE_SAVE,
        ID_MENU_FILE_SAVE_AS,
        ID_MENU_FILE_EXIT,
        ID_MENU_IMAGE_INSERT,
        ID_MENU_IMAGE_DELETE,
        ID_MENU_SPRITE_RENAME,
        ID_MENU_SPRITE_DELETE,
        ID_MENU_SHAPE_RENAME,
        ID_MENU_SHAPE_DELETE
    };

    struct Sprite
    {
        Sprite(const std::string& alias, const Vector2i& position, const Vector2i& size, const ActorShape shapes[]) :
                alias(alias),
                position(position),
                size(size)
        {
            memcpy(this->shapes, shapes, sizeof(this->shapes));
        }

        Sprite(const std::string& alias, const Vector2i& position, const Vector2i& size) :
                alias(alias),
                position(position),
                size(size) {}

        Sprite() {}

        ActorShape  shapes[ACTOR_SHAPE_TYPE_COUNT];
        std::string alias;
        Vector2i    position;
        Vector2i    size;
    };

    struct Image
    {
        Image(const std::string& filename) :
                filename(filename) {}

        Image() {}

        std::string         filename;
        std::vector<Sprite> sprites;
    };

    struct Marquee
    {
        Marquee(const Vector2i& position, const Vector2i& size) :
                position(position),
                size(size) {}

        Marquee() {}

        Vector2i position;
        Vector2i size;
    };

    bool LoadManifest(const wxString& filename);
    bool SaveManifest(const wxString& filename);
    void ClearManifest();

    void UpdateImageList();
    void UpdateSpriteList();
    void UpdateShapeList();
    void UpdateCanvas();

    void InitializeUi();
    void InitializeMenuBar();

    void OnMenuSelectedFileNew(wxCommandEvent& event);
    void OnMenuSelectedFileOpen(wxCommandEvent& event);
    void OnMenuSelectedFileSave(wxCommandEvent& event);
    void OnMenuSelectedFileSaveAs(wxCommandEvent& event);
    void OnMenuSelectedFileExit(wxCommandEvent& event);

    void OnMenuSelectedImageInsert(wxCommandEvent& event);
    void OnMenuSelectedImageDelete(wxCommandEvent& event);
    void OnMenuSelectedSpriteRename(wxCommandEvent& event);
    void OnMenuSelectedSpriteDelete(wxCommandEvent& event);
    void OnMenuSelectedShapeRename(wxCommandEvent& event);
    void OnMenuSelectedShapeDelete(wxCommandEvent& event);

    void OnCanvasLeftDown(wxMouseEvent& event);
    void OnCanvasLeftUp(wxMouseEvent& event);
    void OnCanvasRightDown(wxMouseEvent& event);
    void OnCanvasMotion(wxMouseEvent& event);
    void OnCanvasPaint(wxPaintEvent& event);
    void OnImagesRightDown(wxMouseEvent& event);
    void OnImagesSelected(wxCommandEvent& event);
    void OnSpritesRightDown(wxMouseEvent& event);
    void OnSpritesSelected(wxCommandEvent& event);
    void OnShapesRightDown(wxMouseEvent& event);
    void OnShapesSelected(wxCommandEvent& event);

    void OnModifiedGrid(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    Vector2i AdjustPosition(const Vector2i & position) const;

    DECLARE_EVENT_TABLE()

    wxListBox*          m_listImages;
    wxListBox*          m_listSprites;
    wxListBox*          m_listShapes;
    wxScrolledWindow*   m_scrolledWindow;
    wxPanel*            m_panelCanvas;
    wxTextCtrl*         m_textGrid;

    wxString            m_currentFilename;
    wxBitmap            m_currentBitmap;
    SpriteManifest      m_spriteManager;
    std::vector<Image>  m_images;

    Marquee m_marquee;
    bool    m_draggingSprite;
    bool    m_draggingShape;
    int     m_gridSize;

    int m_imageIndex;
    int m_spriteIndex;
    int m_shapeIndex;
};
