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
#include "LuaBinding.h"

class Script
{
public:
    Script(boost::shared_ptr<const Buffer> script);

    void                            SetParameter(const char* name, const ScriptParameter& parameter);
    bool                            GetParameter(const char* name, ScriptParameter* parameter) const;
    void                            EnumerateParameters(std::vector<std::string>* names) const;
    const                           ParameterMap& GetParameters() const;

    void                            DoString(const char* string);
    void                            ResetState();

protected:
    LuaBinding*                     GetBinding();

private:
    ParameterMap                    m_parameters;
    boost::shared_ptr<const Buffer> m_script;
    LuaBinding                      m_binding;
};
