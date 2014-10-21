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
#include "Script.h"

Script::Script(boost::shared_ptr<const Buffer> script) :
        m_script(script)
{
    m_binding.DoString(static_cast<const char*>(script->Get()));
}

void Script::SetParameter(const char* name, const ScriptParameter& parameter)
{
    ParameterMap::iterator iter = m_parameters.find(name);
    if (iter == m_parameters.end())
    {
        m_parameters.insert(std::make_pair(name, parameter));
    }
    else
    {
        iter->second = parameter;
    }
}

bool Script::GetParameter(const char* name, ScriptParameter* parameter) const
{
    ParameterMap::const_iterator iter = m_parameters.find(name);
    if (iter == m_parameters.end())
    {
        return false;
    }

    *parameter = iter->second;
    return true;
}

void Script::EnumerateParameters(std::vector<std::string>* names) const
{
    for (ParameterMap::const_iterator iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
    {
        names->push_back(iter->first);
    }
}

const ParameterMap& Script::GetParameters() const
{
    return m_parameters;
}

LuaBinding* Script::GetBinding()
{
    return &m_binding;
}

void Script::DoString(const char* string)
{
    m_binding.DoString(string);
}

void Script::ResetState()
{
    m_binding.ResetState();
    m_binding.DoString(static_cast<const char*>(m_script->Get()));
}
