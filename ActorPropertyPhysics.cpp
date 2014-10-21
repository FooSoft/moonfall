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
#include "ActorPropertyPhysics.h"
#include "ActorManager.h"
#include "Actor.h"

const ActorShapeType ActorPropertyPhysics::s_collisionShapes[ACTOR_SHAPE_TYPE_COLLISION_COUNT] =
{
    ACTOR_SHAPE_TYPE_COLLISION_SOLID,
    ACTOR_SHAPE_TYPE_COLLISION_INTERACT
};

ActorPropertyPhysics::ActorPropertyPhysics(Actor* owner) :
        ActorPropertyTyped<ACTOR_PROPERTY_TYPE_PHYSICS>(owner)
{
}

void ActorPropertyPhysics::Update(float elapsed)
{
    Actor* const actor = GetOwner();

    bool collisionShapesPresent = false;
    for (int i = 0; i < ARRAY_SIZE(s_collisionShapes); ++i)
    {
        const ActorShapeType type = s_collisionShapes[i];

        if (actor->IsShapeEnabled(type))
        {
            Vector2i position;
            Vector2i size;
            int data = 0;
            actor->GetShape(type, &data, &position, &size);

            collisionShapesPresent |= (data != 0);
        }

        m_collisionState[type].collisions.clear();
    }

    const Vector2f newPosition = actor->GetPositionSoft() + GetVelocity() * elapsed;

    if (!collisionShapesPresent)
    {
        actor->SetPositionSoft(newPosition);
        return;
    }

    std::vector<boost::weak_ptr<Actor> > actors;
    actor->GetOwner()->EnumerateActors(&actors, actor->GetPosition());

    actor->SetPositionSoft(newPosition);
    for (int i = 0; i < ARRAY_SIZE(s_collisionShapes); ++i)
    {
        CollideAgainstShape(actors, s_collisionShapes[i]);
    }
}

void ActorPropertyPhysics::CollideAgainstShape(const std::vector<boost::weak_ptr<Actor> >& actors, ActorShapeType typeAttacker)
{
    Actor* const actorAttacker = GetOwner();

    Vector2i attackerPosition;
    Vector2i attackerSize;
    int attackerData = 0;
    actorAttacker->GetShape(typeAttacker, &attackerData, &attackerPosition, &attackerSize);

    if (!actorAttacker->IsShapeEnabled(typeAttacker) || attackerData == 0)
    {
        return;
    }

    for (std::vector<boost::weak_ptr<Actor> >::const_iterator iter = actors.begin(); iter != actors.end(); ++iter)
    {
        const boost::shared_ptr<Actor> actorDefender = iter->lock();

        if (!actorDefender || !actorDefender->HasProperty(ACTOR_PROPERTY_TYPE_PHYSICS) || actorDefender->GetId() == actorAttacker->GetId())
        {
            continue;
        }

        for (int i = 0; i < ARRAY_SIZE(s_collisionShapes); ++i)
        {
            const ActorShapeType typeDefender = s_collisionShapes[i];

            Vector2i defenderPosition;
            Vector2i defenderSize;
            int defenderData = 0;
            actorDefender->GetShape(typeDefender, &defenderData, &defenderPosition, &defenderSize);

            if (!actorDefender->IsShapeEnabled(typeDefender) || (attackerData & BIT(typeDefender)) == 0)
            {
                continue;
            }

            const Rect4i rectAttacker(
                actorAttacker->GetPosition().x + attackerPosition.x,
                actorAttacker->GetPosition().y + attackerPosition.y,
                actorAttacker->GetPosition().x + attackerPosition.x + attackerSize.x,
                actorAttacker->GetPosition().y + attackerPosition.y + attackerSize.y
            );

            const Rect4i rectDefender(
                actorDefender->GetPosition().x + defenderPosition.x,
                actorDefender->GetPosition().y + defenderPosition.y,
                actorDefender->GetPosition().x + defenderPosition.x + defenderSize.x,
                actorDefender->GetPosition().y + defenderPosition.y + defenderSize.y
            );

            if (!rectAttacker.Intersects(rectDefender))
            {
                continue;
            }

            if (typeAttacker != ACTOR_SHAPE_TYPE_COLLISION_SOLID || typeDefender != ACTOR_SHAPE_TYPE_COLLISION_SOLID)
            {
                m_collisionState[typeAttacker].collisions.push_back(actorDefender);
                continue;
            }

            const Rect4i intersection = rectAttacker.Intersection(rectDefender);
            Vector2i displacement(0, 0);

            if (GetVelocity().x > 0)
            {
                displacement.x = -intersection.GetWidth();
            }
            else if (GetVelocity().x < 0)
            {
                displacement.x = intersection.GetWidth();
            }

            if (GetVelocity().y > 0)
            {
                displacement.y = -intersection.GetHeight();
            }
            else if (GetVelocity().y < 0)
            {
                displacement.y = intersection.GetHeight();
            }

            actorAttacker->SetPosition(actorAttacker->GetPosition() + displacement);
        }
    }
}

void ActorPropertyPhysics::SetVelocity(const Vector2f& velocity)
{
    m_velocity = velocity;
}

Vector2f ActorPropertyPhysics::GetVelocity() const
{
    return m_velocity;
}

void ActorPropertyPhysics::QueryShapeCollisions(ActorShapeType type, std::vector<boost::weak_ptr<Actor> >* actors) const
{
    ASSERT(type < ACTOR_SHAPE_TYPE_COUNT);
    const CollisionState& state = m_collisionState[type];
    for (std::vector<boost::weak_ptr<Actor> >::const_iterator iter = state.collisions.begin(); iter != state.collisions.end(); ++iter)
    {
        if (!iter->expired())
        {
            actors->push_back(*iter);
        }
    }
}
