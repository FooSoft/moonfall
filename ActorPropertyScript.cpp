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
#include "ActorPropertyScript.h"
#include "ActorScript.h"
#include "Actor.h"

ActorPropertyScript::ActorPropertyScript(Actor* owner) :
        ActorPropertyTyped<ACTOR_PROPERTY_TYPE_SCRIPT>(owner),
        m_queuedCreate(false)
{
}

ActorPropertyScript::~ActorPropertyScript()
{
    SetScript(NULL);
}

void ActorPropertyScript::Update(float elapsed)
{
    if (m_script && System::QueryWorldContext() != NULL)
    {
        if (m_queuedCreate)
        {
            m_script->OnActorCreate(GetOwner()->GetId());
            m_queuedCreate = false;
        }

        m_script->OnActorUpdate(elapsed);
    }
}

void ActorPropertyScript::SetScript(const char* resource)
{
    if (m_script)
    {
        m_script->OnActorDestroy();
    }

    if (resource == NULL || *resource == '\0')
    {
        m_script.reset();
    }
    else
    {
        m_script = System::LoadActorScript(resource);
    }

    m_queuedCreate = m_script;
}

bool ActorPropertyScript::SendMessage(const char* message, const ParameterMap& parametersIn, ParameterMap* parametersOut)
{
    return m_script->OnActorReceiveMessage(message, parametersIn, parametersOut);
}

void ActorPropertyScript::ResetScriptState()
{
    if (m_script)
    {
        m_script->ResetState();
        m_queuedCreate = true;
    }
}

void ActorPropertyScript::SetParameter(const char* name, const ScriptParameter& parameter)
{
    if (m_script)
    {
        m_script->SetParameter(name, parameter);
    }
}

bool ActorPropertyScript::GetParameter(const char* name, ScriptParameter* parameter) const
{
    return m_script ? m_script->GetParameter(name, parameter) : false;
}

void ActorPropertyScript::EnumerateParameters(std::vector<std::string>* names) const
{
    if (m_script)
    {
        m_script->EnumerateParameters(names);
    }
}
