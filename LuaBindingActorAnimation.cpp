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
#include "ActorPropertyAnimation.h"

int LuaBinding::ActorAnimationSet()
{
    int actorId = 0;
    const char* animation = NULL;
    if (!Peek(1, &actorId) || !Peek(2, &animation))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->SetAnimation(animation);
    return 0;
}

int LuaBinding::ActorAnimationPlay()
{
    int actorId = 0;
    bool loop = false;
    if (!Peek(1, &actorId) || !Peek(2, &loop))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->PlayAnimation(loop);
    return 0;
}

int LuaBinding::ActorAnimationIsPlaying()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    Push(property->IsAnimationPlaying());
    return 1;
}

int LuaBinding::ActorAnimationStop()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->StopAnimation();
    return 0;
}

int LuaBinding::ActorAnimationRewind()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->RewindAnimation();
    return 0;
}

int LuaBinding::ActorAnimationPause()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->PauseAnimation();
    return 0;
}

int LuaBinding::ActorAnimationGetSize()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyAnimation> property = LuaBinding::GetActorProperty<ActorPropertyAnimation>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    Push(property->GetSize());
    return 1;
}
