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

class ScriptManifest
{
public:
    bool                            LoadManifest(const char* filename);
    void                            ClearManifest();

    template <typename T>
    boost::shared_ptr<T>            LoadScript(const char* scriptAlias) const;
    boost::shared_ptr<const Buffer> LoadScriptCode(const char* scriptAlias) const;
    void                            SummarizeScripts(std::vector<ScriptSummary>* summary) const;

private:
    struct ScriptEntry
    {
        ScriptEntry(const std::string& filename) :
                filename(filename) { }

        std::string                 filename;
        ParameterMap                parameters;
    };

    typedef                         std::map<std::string, ScriptEntry> ScriptMap;
    ScriptMap                       m_manifest;
};

#include "ScriptManifest.inl"
