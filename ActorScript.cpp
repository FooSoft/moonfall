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
#include "ActorScript.h"

ActorScript::ActorScript(const boost::shared_ptr<const Buffer>& script) :
        Script(script)
{
}

void ActorScript::OnActorCreate(Token id)
{
    GetBinding()->GetGlobal("OnActorCreate");
    if (!GetBinding()->IsFunction(GetBinding()->GetTop()))
    {
        GetBinding()->Pop(1);
        return;
    }

    GetBinding()->Push(id);
    GetBinding()->Push(GetParameters());
    GetBinding()->Call(2, 0);
}

void ActorScript::OnActorDestroy()
{
    GetBinding()->GetGlobal("OnActorDestroy");
    if (!GetBinding()->IsFunction(GetBinding()->GetTop()))
    {
        GetBinding()->Pop(1);
        return;
    }

    GetBinding()->Call(0, 0);
}

void ActorScript::OnActorUpdate(float elapsed)
{
    GetBinding()->GetGlobal("OnActorUpdate");
    if (!GetBinding()->IsFunction(GetBinding()->GetTop()))
    {
        GetBinding()->Pop(1);
        return;
    }

    GetBinding()->Push(elapsed);
    GetBinding()->Call(1, 0);
}

bool ActorScript::OnActorReceiveMessage(const char* message, const ParameterMap& parametersIn, ParameterMap* parametersOut)
{
    GetBinding()->GetGlobal("OnActorReceiveMessage");
    if (!GetBinding()->IsFunction(GetBinding()->GetTop()))
    {
        GetBinding()->Pop(1);
        return false;
    }

    GetBinding()->Push(message);
    GetBinding()->Push(parametersIn);
    GetBinding()->Call(2, 1);

    const bool responded = GetBinding()->Peek(GetBinding()->GetTop(), parametersOut);
    GetBinding()->Pop(1);
    return responded;
}
