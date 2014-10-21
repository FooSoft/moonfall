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

class FontManifest
{
public:
    bool                    LoadManifest(const char* filename);
    void                    ClearManifest();

    boost::shared_ptr<Font> LoadFont(const char* fontAlias) const;

private:
    struct FontEntry
    {
        FontEntry(const std::string& filename, const Color4b& color, int size) :
                filename(filename), color(color), size(size) { }

        std::string         filename;
        Color4b             color;
        int                 size;
    };

    typedef                 std::map<std::string, FontEntry> FontMap;
    FontMap                 m_manifest;
};
