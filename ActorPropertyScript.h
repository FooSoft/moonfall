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
#include "ActorProperty.h"

class ActorPropertyScript : public ActorPropertyTyped<ACTOR_PROPERTY_TYPE_SCRIPT>
{
public:
    ActorPropertyScript(Actor* owner);
    ~ActorPropertyScript();

    void                            Update(float elapsed);

    void                            SetScript(const char* resource);
    bool                            SendMessage(const char* message, const ParameterMap& parametersIn, ParameterMap* parametersOut);
    void                            ResetScriptState();

    void                            SetParameter(const char* name, const ScriptParameter& parameter);
    bool                            GetParameter(const char* name, ScriptParameter* parameter) const;
    void                            EnumerateParameters(std::vector<std::string>* names) const;

private:
    boost::shared_ptr<ActorScript>  m_script;
    bool                            m_queuedCreate;
};
