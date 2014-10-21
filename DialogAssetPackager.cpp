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
#include "DialogAssetPackager.h"

BEGIN_EVENT_TABLE(DialogAssetPackager, wxDialog)
    EVT_BUTTON(XRCID("buttonBrowseDirectory"),  DialogAssetPackager::OnButtonBrowseDirectory)
    EVT_BUTTON(XRCID("buttonBrowseFilename"),   DialogAssetPackager::OnButtonBrowseFilename)
    EVT_BUTTON(wxID_OK,                         DialogAssetPackager::OnButtonOk)
END_EVENT_TABLE()

DialogAssetPackager::DialogAssetPackager(wxWindow* parent, const wxString& assetsDirectory, const wxString& packageFilename) :
        m_textDirectory(NULL),
        m_textFilename(NULL),
        m_listFilenames(NULL)
{
    wxXmlResource::Get()->LoadDialog(this, parent, wxT("DialogAssetPackager"));

    m_textDirectory = XRCCTRL(*this, "textDirectory", wxTextCtrl);
    m_textFilename  = XRCCTRL(*this, "textFilename", wxTextCtrl);
    m_listFilenames = XRCCTRL(*this, "listFilenames", wxListBox);

    SetAssetDirectory(assetsDirectory);
    SetPackageFilename(packageFilename);
    SetSize(500, 500);
}

void DialogAssetPackager::SetAssetDirectory(const wxString& directory)
{
    m_listFilenames->Clear();

    wxFileName searchPath = wxFileName::DirName(directory);
    if (!searchPath.MakeRelativeTo(wxGetCwd()) || !searchPath.DirExists())
    {
        m_textDirectory->SetValue(wxEmptyString);
        return;
    }

    wxArrayString filenames;
    wxDir directoryTemp;
    directoryTemp.GetAllFiles(searchPath.GetFullPath(), &filenames, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);

    m_textDirectory->SetValue(searchPath.GetFullPath());
    m_listFilenames->Append(filenames);
}

void DialogAssetPackager::SetPackageFilename(const wxString& filename)
{
    wxFileName filenamePack = filename;
    filenamePack.MakeRelativeTo(wxGetCwd());
    m_textFilename->SetValue(filenamePack.GetFullPath());
}

void DialogAssetPackager::OnButtonBrowseDirectory(wxCommandEvent& event)
{
    wxDirDialog dialog(this, wxT("Choose a directory to package"));
    if (dialog.ShowModal() == wxID_OK)
    {
        SetAssetDirectory(dialog.GetPath());
    }
}

void DialogAssetPackager::OnButtonBrowseFilename(wxCommandEvent& event)
{
    wxFileDialog dialog(
        this,
        wxT("Choose a package file"),
        wxEmptyString,
        wxEmptyString,
        wxT("Package files (*.pak)|*.pak|All files (*.*)|*.*"),
        wxFD_SAVE
    );

    if (dialog.ShowModal() == wxID_OK)
    {
        SetPackageFilename(dialog.GetPath());
    }
}

void DialogAssetPackager::OnButtonOk(wxCommandEvent& event)
{
    const wxArrayString filenamesUnicode = m_listFilenames->GetStrings();
    if (filenamesUnicode.GetCount() == 0)
    {
        return;
    }

    std::vector<std::string> filenamesAscii;
    for (size_t i = 0; i < filenamesUnicode.Count(); ++i)
    {
        filenamesAscii.push_back(filenamesUnicode[i].ToAscii().data());
    }

    const wxString packageFilename = m_textFilename->GetValue();
    if (packageFilename.IsEmpty())
    {
        return;
    }

    if (System::SaveFileManifest(packageFilename.ToAscii(), filenamesAscii))
    {
        wxMessageBox(wxT("Packing complete!"), wxT("Moonfall"), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        wxMessageBox(wxT("Packing failed!"), wxT("Moonfall"), wxOK | wxICON_ERROR, this);
    }

    EndModal(wxID_OK);
}
