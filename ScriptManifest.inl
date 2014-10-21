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

template <typename T>
boost::shared_ptr<T> ScriptManifest::LoadScript(const char* scriptAlias) const
{
    ScriptMap::const_iterator iter = m_manifest.find(scriptAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load script %s") % scriptAlias);
        return boost::shared_ptr<T>();
    }

    boost::shared_ptr<const Buffer> buffer = System::LoadFile(iter->second.filename.c_str());
    if (!buffer)
    {
        return boost::shared_ptr<T>();
    }

    boost::shared_ptr<T> script(new T(buffer));
    for (ParameterMap::const_iterator paramIter = iter->second.parameters.begin(); paramIter != iter->second.parameters.end(); ++paramIter)
    {
        script->SetParameter(paramIter->first.c_str(), paramIter->second);
    }

    return script;
}
