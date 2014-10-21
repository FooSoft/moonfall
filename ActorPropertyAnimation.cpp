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
#include "ActorPropertyAnimation.h"
#include "Animation.h"
#include "Surface.h"
#include "Actor.h"

ActorPropertyAnimation::ActorPropertyAnimation(Actor* owner) :
        ActorPropertyTyped<ACTOR_PROPERTY_TYPE_ANIMATION>(owner)
{
}

void ActorPropertyAnimation::Render(const boost::shared_ptr<Surface>& target)
{
    if (m_animation)
    {
        m_animation->Render(target);
    }
}

void ActorPropertyAnimation::Update(float elapsed)
{
    bool frameChanged = false;

    if (m_animation)
    {
        const int frame = m_animation->GetFrame();
        m_animation->Update(elapsed);
        frameChanged = frame != m_animation->GetFrame();
    }

    if (frameChanged)
    {
        UpdateShapes();
    }
}

void ActorPropertyAnimation::SetAnimation(const char* resource)
{
    m_animation = System::LoadAnimation(resource);
    UpdateShapes();
}

void ActorPropertyAnimation::PlayAnimation(bool loop)
{
    if (m_animation)
    {
        m_animation->Play(loop);
    }
}

bool ActorPropertyAnimation::IsAnimationPlaying() const
{
    return m_animation && m_animation->IsPlaying();
}

void ActorPropertyAnimation::StopAnimation()
{
    if (m_animation)
    {
        m_animation->Stop();
    }
}

void ActorPropertyAnimation::RewindAnimation()
{
    if (m_animation)
    {
        m_animation->Rewind();
    }
}

void ActorPropertyAnimation::PauseAnimation()
{
    if (m_animation)
    {
        m_animation->Pause();
    }
}

Vector2i ActorPropertyAnimation::GetSize() const
{
    return m_animation ? m_animation->GetSize() : Vector2i();
}

void ActorPropertyAnimation::UpdateShapes()
{
    for (int i = 0; i < ACTOR_SHAPE_TYPE_COUNT; ++i)
    {
        GetOwner()->DisableShape(static_cast<ActorShapeType>(i));
    }

    if (!m_animation)
    {
        return;
    }

    const ActorShape* shapes = m_animation->GetShapes();
    for (size_t i = 0; i < ACTOR_SHAPE_TYPE_COUNT; ++i)
    {
        GetOwner()->SetShape(static_cast<ActorShapeType>(i), shapes[i]);
    }
}
