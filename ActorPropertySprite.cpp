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
#include "ActorPropertySprite.h"
#include "Sprite.h"
#include "Surface.h"
#include "Actor.h"

ActorPropertySprite::ActorPropertySprite(Actor* owner) :
        ActorPropertyTyped<ACTOR_PROPERTY_TYPE_SPRITE>(owner)
{
}

void ActorPropertySprite::Render(const boost::shared_ptr<Surface>& target)
{
    if (m_sprite)
    {
        m_sprite->Render(target);
    }
}

void ActorPropertySprite::SetSprite(const char* resource)
{
    m_sprite = System::LoadSprite(resource);
    UpdateShapes();
}

Vector2i ActorPropertySprite::GetSize() const
{
    return m_sprite ? m_sprite->GetSize() : Vector2i();
}

void ActorPropertySprite::UpdateShapes()
{
    for (int i = 0; i < ACTOR_SHAPE_TYPE_COUNT; ++i)
    {
        GetOwner()->DisableShape(static_cast<ActorShapeType>(i));
    }

    if (!m_sprite)
    {
        return;
    }

    const ActorShape* shapes = m_sprite->GetShapes();
    for (size_t i = 0; i < ACTOR_SHAPE_TYPE_COUNT; ++i)
    {
        GetOwner()->SetShape(static_cast<ActorShapeType>(i), shapes[i]);
    }
}
