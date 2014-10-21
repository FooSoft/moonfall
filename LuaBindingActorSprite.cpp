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
#include "ActorPropertySprite.h"

int LuaBinding::ActorSpriteSet()
{
    int actorId = 0;
    const char* sprite = NULL;
    if (!Peek(1, &actorId) || !Peek(2, &sprite))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertySprite> property = LuaBinding::GetActorProperty<ActorPropertySprite>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->SetSprite(sprite);
    return 0;
}

int LuaBinding::ActorSpriteGetSize()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertySprite> property = LuaBinding::GetActorProperty<ActorPropertySprite>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    Push(property->GetSize());
    return 1;
}
