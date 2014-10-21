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
#include "LuaBinding.h"
#include "ActorPropertyScript.h"

int LuaBinding::ActorScriptSendMessage()
{
    int actorId = 0;
    const char* message = NULL;
    if (!Peek(1, &actorId) || !Peek(2, &message))
    {
        return ERROR_TYPE_PARAMETER;
    }

    ParameterMap parametersIn;
    Peek(3, &parametersIn);

    const boost::shared_ptr<ActorPropertyScript> property = GetActorProperty<ActorPropertyScript>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    ParameterMap parametersOut;
    if (!property->SendMessage(message, parametersIn, &parametersOut))
    {
        return 0;
    }

    Push(parametersOut);
    return 1;
}
