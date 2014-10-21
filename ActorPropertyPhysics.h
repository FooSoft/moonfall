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

class ActorPropertyPhysics : public ActorPropertyTyped<ACTOR_PROPERTY_TYPE_PHYSICS>
{
public:
    ActorPropertyPhysics(Actor* owner);

    void                        Update(float elapsed);

    void                        SetVelocity(const Vector2f& velocity);
    Vector2f                    GetVelocity() const;
    void                        QueryShapeCollisions(ActorShapeType type, std::vector<boost::weak_ptr<Actor> >* actors) const;

private:
    void                        UpdateShapes();
    void                        CollideAgainstShape(const std::vector<boost::weak_ptr<Actor> >& actors, ActorShapeType typeAttacker);

    struct CollisionState
    {
        std::vector<boost::weak_ptr<Actor> > collisions;
    };

    static const ActorShapeType s_collisionShapes[ACTOR_SHAPE_TYPE_COLLISION_COUNT];
    CollisionState              m_collisionState[ACTOR_SHAPE_TYPE_COUNT];
    Vector2f                    m_velocity;
};
