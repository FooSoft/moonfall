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
#include "ActorPropertyPhysics.h"

int LuaBinding::ActorPhysicsGetVelocity()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyPhysics> property = LuaBinding::GetActorProperty<ActorPropertyPhysics>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    Push(property->GetVelocity());
    return 1;
}

int LuaBinding::ActorPhysicsSetVelocity()
{
    int actorId = 0;
    Vector2f velocity;
    if (!Peek(1, &actorId) || !Peek(2, &velocity))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyPhysics> property = LuaBinding::GetActorProperty<ActorPropertyPhysics>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    property->SetVelocity(velocity);
    return 0;
}

int LuaBinding::ActorPhysicsQueryShapeCollisions()
{
    int actorId = 0;
    int actorShape = 0;
    if (!Peek(1, &actorId) || !Peek(2, &actorShape) || actorShape >= ACTOR_SHAPE_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<ActorPropertyPhysics> property = LuaBinding::GetActorProperty<ActorPropertyPhysics>(actorId);
    if (!property)
    {
        return ERROR_TYPE_STATE;
    }

    std::vector<boost::weak_ptr<Actor> > actors;
    property->QueryShapeCollisions(static_cast<ActorShapeType>(actorShape), &actors);

    std::vector<int> actorIds;
    for (std::vector<boost::weak_ptr<Actor> >::const_iterator iter = actors.begin(); iter != actors.end(); ++iter)
    {
        const boost::shared_ptr<Actor> actor = iter->lock();
        if (actor)
        {
            actorIds.push_back(actor->GetId());
        }
    }

    Push(actorIds);
    return 1;
}
