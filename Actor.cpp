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
#include "Actor.h"
#include "ActorProperty.h"
#include "ActorManager.h"
#include "Surface.h"

#define ACTOR_ID_FROM_NAME(name) (Token(name) | ~0x7fffffff)
#define ACTOR_ID_FROM_INC(inc) (Token(inc) & 0x7fffffff)

Actor::Actor(const char* alias, const char* name) :
        m_owner(NULL),
        m_dynamic(false),
        m_layer(ACTOR_LAYER_COUNT)
{
    if (alias != NULL)
    {
        m_alias = alias;
    }

    SetName(name);
}

void Actor::Render(const boost::shared_ptr<Surface>& target)
{
    for (int i = 0; i < ARRAY_SIZE(m_properties); ++i)
    {
        if (m_properties[i])
        {
            m_properties[i]->Render(target);
        }
    }

    if (m_owner->AreShapesVisible())
    {
        RenderShapes(target);
    }
}

void Actor::Update(float elapsed)
{
    for (int i = 0; i < ARRAY_SIZE(m_properties); ++i)
    {
        if (m_properties[i])
        {
            m_properties[i]->Update(elapsed);
        }
    }
}

void Actor::SetPosition(const Vector2i& position)
{
    const Vector2f positionFloat(static_cast<float>(position.x), static_cast<float>(position.y));
    SetPositionSoft(positionFloat);
}

Vector2i Actor::GetPosition() const
{
    const Vector2i positionInt(static_cast<int>(floorf(m_position.x + 0.5f)), static_cast<int>(floorf(m_position.y + 0.5f)));
    return positionInt;
}

void Actor::SetPositionSoft(const Vector2f& position)
{
    const Vector2i positionInt(static_cast<int>(floorf(position.x + 0.5f)), static_cast<int>(floorf(position.y + 0.5f)));

    if (positionInt != GetPosition() && m_owner != NULL)
    {
        m_owner->UpdateActorPosition(GetId(), GetPosition(), positionInt);
    }

    m_position = position;
}

Vector2f Actor::GetPositionSoft() const
{
    return m_position;
}

void Actor::SetLayer(ActorLayer layer)
{
    m_layer = layer;
}

ActorLayer Actor::GetLayer() const
{
    return m_layer;
}

void Actor::SetDynamic(bool dynamic)
{
    m_dynamic = dynamic;
}

bool Actor::IsDynamic() const
{
    return m_dynamic;
}

void Actor::SetId(Token id)
{
    if (id == GetId())
    {
        return;
    }

    if (m_owner != NULL && GetId().IsValid())
    {
        m_owner->UpdateActorId(GetId(), id);
    }

    m_id = id;
}

Token Actor::GetId() const
{
    return m_id;
}

const char* Actor::GetAlias() const
{
    return m_alias.c_str();
}

void Actor::SetName(const char* name)
{
    if (name == NULL || strlen(name) == 0)
    {
        SetId(ACTOR_ID_FROM_INC(RegisterId()));
        m_name.clear();
    }
    else
    {
        SetId(ACTOR_ID_FROM_NAME(name));
        m_name = name;
    }
}

const char* Actor::GetName() const
{
    return m_name.c_str();
}

void Actor::SetOwner(ActorManager* owner)
{
    m_owner = owner;
}

const ActorManager* Actor::GetOwner() const
{
    return m_owner;
}

ActorManager* Actor::GetOwner()
{
    return m_owner;
}

void Actor::GetShape(ActorShapeType type, int* data, Vector2i* position, Vector2i* size) const
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    *data = m_shapes[type].data;
    *position = m_shapes[type].position;
    *size = m_shapes[type].size;
}

void Actor::SetShape(ActorShapeType type, int data, const Vector2i& position, const Vector2i& size)
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    m_shapes[type].data = data;
    m_shapes[type].position = position;
    m_shapes[type].size = size;
}

ActorShape Actor::GetShape(ActorShapeType type) const
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    return m_shapes[type];
}

void Actor::SetShape(ActorShapeType type, const ActorShape& shape)
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    m_shapes[type] = shape;
}

void Actor::EnableShape(ActorShapeType type)
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    m_shapes[type].enabled = true;
}

void Actor::DisableShape(ActorShapeType type)
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    m_shapes[type].enabled = false;
}

bool Actor::IsShapeEnabled(ActorShapeType type) const
{
    ASSERT(type < ARRAY_SIZE(m_shapes));
    return m_shapes[type].enabled;
}

Token Actor::RegisterId()
{
    static int s_idPool = 0;
    Token token(s_idPool);
    do
    {
        token = Token(++s_idPool);
    }
    while (!token);
    return token;
}

void Actor::RenderShapes(const boost::shared_ptr<Surface>& target)
{
    Vector2i maxShapeSize(0, 0);
    for (int i = 0; i < ARRAY_SIZE(m_shapes); ++i)
    {
        const ActorShape& shape = m_shapes[i];
        if (!shape.enabled)
        {
            continue;
        }

        maxShapeSize.x = MAX(maxShapeSize.x, shape.size.x);
        maxShapeSize.y = MAX(maxShapeSize.y, shape.size.y);
    }

    if (maxShapeSize.x == 0 || maxShapeSize.y == 0)
    {
        return;
    }

    if (!m_shapeSurface || m_shapeSurface->GetSize().x < maxShapeSize.x || m_shapeSurface->GetSize().y < maxShapeSize.y)
    {
        m_shapeSurface = System::CreateSurface(maxShapeSize, COLOR_MODE_RGBA_8888);
    }

    static const Color4b s_shapeColors[] =
    {
        Color4b(0x00, 0x00, 0xc0, 0x60), // ACTOR_SHAPE_TYPE_COLLISION_SOLID
        Color4b(0x00, 0xc0, 0x00, 0x60)  // ACTOR_SHAPE_TYPE_COLLISION_INTERACT
    };

    BOOST_STATIC_ASSERT(ARRAY_SIZE(s_shapeColors) == ACTOR_SHAPE_TYPE_COUNT);

    for (int i = 0; i < ARRAY_SIZE(m_shapes); ++i)
    {
        const ActorShape& shape = m_shapes[i];
        if (!shape.enabled)
        {
            continue;
        }

        target->PushState();
        target->SetTranslationRelative(shape.position);
        m_shapeSurface->Clear(s_shapeColors[i]);
        m_shapeSurface->Blit(target, Vector2i(0, 0), shape.size);
        target->PopState();
    }
}
