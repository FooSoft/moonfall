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
#include "IWorldContext.h"

int LuaBinding::ActorAdd()
{
    const char* actorAlias = NULL;
    Vector2i actorPosition;
    if (!Peek(1, &actorAlias) || !Peek(2, &actorPosition))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const char* actorName = NULL;
    Peek(3, &actorName);

    ParameterMap parameters;
    Peek(4, &parameters);

    IWorldContext* worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    const boost::shared_ptr<Actor> actor = worldContext->AddActor(actorAlias, actorPosition, actorName).lock();
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    const boost::shared_ptr<ActorPropertyScript> property = actor->GetProperty<ActorPropertyScript>().lock();
    if (property)
    {
        for (ParameterMap::iterator iter = parameters.begin(); iter != parameters.end(); ++iter)
        {
            property->SetParameter(iter->first.c_str(), iter->second);
        }
    }

    Push(actor->GetId());
    return 1;
}

int LuaBinding::ActorRemove()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    IWorldContext* worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    worldContext->RemoveActor(actorId);
    return 0;
}

int LuaBinding::ActorGetPosition()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->GetPosition());
    return 1;
}

int LuaBinding::ActorSetPosition()
{
    int actorId = 0;
    Vector2i position;
    if (!Peek(1, &actorId) || !Peek(2, &position))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    actor->SetPosition(position);
    return 0;
}

int LuaBinding::ActorGetLayer()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->GetLayer());
    return 1;
}

int LuaBinding::ActorSetLayer()
{
    int actorId = 0;
    int actorLayer = ACTOR_LAYER_COUNT;
    if (!Peek(1, &actorId) || !Peek(2, &actorLayer) || actorLayer >= ACTOR_LAYER_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    actor->SetLayer(static_cast<ActorLayer>(actorLayer));
    return 0;
}

int LuaBinding::ActorValidate()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    Push(actor);
    return 1;
}

int LuaBinding::ActorFromName()
{
    const char* actorName = NULL;
    if (!Peek(1, &actorName))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorName);
    if (actor)
    {
        Push(actor->GetId());
        return 1;
    }

    return 0;
}

int LuaBinding::ActorGetName()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->GetName());
    return 1;
}

int LuaBinding::ActorGetAlias()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->GetAlias());
    return 1;
}

int LuaBinding::ActorHasProperty()
{
    int actorId = 0;
    int actorPropertyType = ACTOR_PROPERTY_TYPE_COUNT;
    if (!Peek(1, &actorId) || !Peek(2, &actorPropertyType) || actorPropertyType >= ACTOR_PROPERTY_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->HasProperty(static_cast<ActorPropertyType>(actorPropertyType)));
    return 1;
}

int LuaBinding::ActorGetShape()
{
    int actorId = 0;
    int actorShapeType = ACTOR_SHAPE_TYPE_COUNT;
    if (!Peek(1, &actorId) || !Peek(2, &actorShapeType) || actorShapeType >= ACTOR_SHAPE_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->GetShape(static_cast<ActorShapeType>(actorShapeType)));
    return 1;
}

int LuaBinding::ActorSetShape()
{
    int actorId = 0;
    int actorShapeType = ACTOR_SHAPE_TYPE_COUNT;
    ActorShape actorShape;
    if (!Peek(1, &actorId) || !Peek(2, &actorShapeType) || !Peek(3, &actorShape) || actorShapeType >= ACTOR_SHAPE_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    actor->SetShape(static_cast<ActorShapeType>(actorShapeType), actorShape);
    return 1;
}

int LuaBinding::ActorEnableShape()
{
    int actorId = 0;
    int actorShapeType = ACTOR_SHAPE_TYPE_COUNT;
    if (!Peek(1, &actorId) || !Peek(2, &actorShapeType) || actorShapeType >= ACTOR_SHAPE_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    actor->EnableShape(static_cast<ActorShapeType>(actorShapeType));
    return 0;
}

int LuaBinding::ActorDisableShape()
{
    int actorId = 0;
    int actorShapeType = ACTOR_SHAPE_TYPE_COUNT;
    if (!Peek(1, &actorId) || !Peek(2, &actorShapeType) || actorShapeType >= ACTOR_SHAPE_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    actor->DisableShape(static_cast<ActorShapeType>(actorShapeType));
    return 0;
}

int LuaBinding::ActorIsShapeEnabled()
{
    int actorId = 0;
    int actorShapeType = ACTOR_SHAPE_TYPE_COUNT;
    if (!Peek(1, &actorId) || !Peek(2, &actorShapeType) || actorShapeType >= ACTOR_SHAPE_TYPE_COUNT)
    {
        return ERROR_TYPE_PARAMETER;
    }

    const boost::shared_ptr<Actor> actor = GetActor(actorId);
    if (!actor)
    {
        return ERROR_TYPE_STATE;
    }

    Push(actor->IsShapeEnabled(static_cast<ActorShapeType>(actorShapeType)));
    return 1;
}
