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

class DialogAssetPackager : public wxDialog
{
public:
    DialogAssetPackager(wxWindow* parent, const wxString& assetsDirectory = wxEmptyString, const wxString& packageFilename = wxEmptyString);

private:
    void            SetAssetDirectory(const wxString& directory);
    void            SetPackageFilename(const wxString& filename);

    void            OnButtonBrowseDirectory(wxCommandEvent& event);
    void            OnButtonBrowseFilename(wxCommandEvent& event);
    void            OnButtonOk(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

    wxTextCtrl*     m_textDirectory;
    wxTextCtrl*     m_textFilename;
    wxListBox*      m_listFilenames;
};
