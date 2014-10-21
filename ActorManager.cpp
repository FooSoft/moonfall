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
#include "ActorManager.h"
#include "Actor.h"
#include "ActorPropertyAnimation.h"
#include "ActorPropertySprite.h"
#include "Surface.h"

ActorManager::ActorManager() :
        m_layers(static_cast<unsigned>(-1)),
        m_renderShapes(false)
{
}

void ActorManager::Render(const boost::shared_ptr<Surface>& target, const Rect4i& bounds)
{
    for (size_t i = 0; i < m_actorCache.size(); ++i)
    {
        const boost::shared_ptr<Actor> actor = m_actorCache[i];
        if (IsLayerVisible(actor->GetLayer()))
        {
            target->SetTranslation(Vector2i(actor->GetPosition().x - bounds.x0, actor->GetPosition().y - bounds.y0));
            actor->Render(target);
        }
    }
}

void ActorManager::Update(float elapsed, const Rect4i& bounds)
{
    m_actorCache.clear();
    m_actors.Query(&m_actorCache, bounds + Rect4i(-256, -256, 256, 256));
    std::sort(m_actorCache.begin(), m_actorCache.end(), ActorSorter);

    for (size_t i = 0; i < m_actorCache.size(); ++i)
    {
        m_actorCache[i]->Update(elapsed);
    }
}

boost::weak_ptr<Actor> ActorManager::AddActor(const char* actorAlias, const Vector2i& actorPosition, const char* actorName)
{
    do
    {
        const boost::shared_ptr<Actor> actor = System::CreateActor(actorAlias, actorName);
        if (!actor)
        {
            break;
        }

        actor->SetPosition(actorPosition);
        actor->SetOwner(this);

        if (!m_actors.Add(actor, actor->GetId(), actor->GetPosition()))
        {
            TRACE_ERROR(
                boost::format("Cannot add actor %s with id %d at (%f, %f)") %
                actorAlias %
                actor->GetId() %
                actorPosition.x %
                actorPosition.y
            );
            break;
        }

        return actor;
    }
    while (false);

    return boost::weak_ptr<Actor>();
}

boost::weak_ptr<const Actor> ActorManager::GetActor(Token actorId) const
{
    const boost::shared_ptr<Actor>* const actor = m_actors.Get(actorId);
    return actor == NULL ? boost::weak_ptr<Actor>() : *actor;
}

boost::weak_ptr<Actor> ActorManager::GetActor(Token actorId)
{
    const boost::shared_ptr<Actor>* const actor = m_actors.Get(actorId);
    return actor == NULL ? boost::weak_ptr<Actor>() : *actor;
}

boost::weak_ptr<const Actor> ActorManager::GetActor(const char* actorName) const
{
    return GetActor(Token(actorName) | ~0x7fffffff);
}

boost::weak_ptr<Actor> ActorManager::GetActor(const char* actorName)
{
    return GetActor(Token(actorName) | ~0x7fffffff);
}

boost::weak_ptr<const Actor> ActorManager::PickActor(const Vector2i& position, const Vector2i& extents) const
{
    return const_cast<ActorManager*>(this)->PickActor(position);
}

boost::weak_ptr<Actor> ActorManager::PickActor(const Vector2i& position, const Vector2i& extents)
{
    const Rect4i boundsQuery(
        position.x - extents.x,
        position.y - extents.y,
        position.x + extents.x,
        position.y + extents.y
    );

    std::vector<boost::shared_ptr<Actor> > actors;
    m_actors.Query(&actors, boundsQuery);
    std::sort(actors.begin(), actors.end(), ActorSorter);

    for (int i = static_cast<int>(actors.size()) - 1; i >= 0; --i)
    {
        const boost::shared_ptr<Actor> actor = actors[i];
        if (!IsLayerVisible(actor->GetLayer()))
        {
            continue;
        }

        const Vector2i actorPosition = actor->GetPosition();

        if (boost::shared_ptr<ActorPropertySprite> property = actor->GetProperty<ActorPropertySprite>().lock())
        {
            const Rect4i actorRect(
                actorPosition.x,
                actorPosition.y,
                actorPosition.x + property->GetSize().x,
                actorPosition.y + property->GetSize().y
            );

            if (actorRect.Contains(position.x, position.y))
            {
                return actor;
            }
        }

        if (boost::shared_ptr<ActorPropertyAnimation> property = actor->GetProperty<ActorPropertyAnimation>().lock())
        {
            const Rect4i actorRect(
                actorPosition.x,
                actorPosition.y,
                actorPosition.x + property->GetSize().x,
                actorPosition.y + property->GetSize().y
            );

            if (actorRect.Contains(position.x, position.y))
            {
                return actor;
            }
        }

        if (AreShapesVisible())
        {
            for (int i = 0; i < ACTOR_SHAPE_TYPE_COUNT; ++i)
            {
                ActorShape actorShape = actor->GetShape(static_cast<ActorShapeType>(i));
                if (!actorShape.enabled)
                {
                    continue;
                }

                const Rect4i actorRect(
                    actorPosition.x + actorShape.position.x,
                    actorPosition.y + actorShape.position.y,
                    actorPosition.x + actorShape.position.x + actorShape.size.x,
                    actorPosition.y + actorShape.position.y + actorShape.size.y
                );

                if (actorRect.Contains(position.x, position.y))
                {
                    return actor;
                }
            }
        }
    }

    return boost::weak_ptr<Actor>();
}

void ActorManager::EnumerateActors(std::vector<boost::weak_ptr<Actor> >* actors)
{
    std::vector<boost::shared_ptr<Actor> > temp;
    m_actors.Enumerate(&temp);
    for (size_t i = 0; i < temp.size(); ++i)
    {
        actors->push_back(temp[i]);
    }
}

void ActorManager::EnumerateActors(std::vector<boost::weak_ptr<Actor> >* actors, const Vector2i& position, const Vector2i& extents)
{
    const Rect4i boundsQuery(
        position.x - extents.x,
        position.y - extents.y,
        position.x + extents.x,
        position.y + extents.y
    );

    std::vector<boost::shared_ptr<Actor> > boundActors;
    m_actors.Query(&boundActors, boundsQuery);

    for (std::vector<boost::shared_ptr<Actor> >::iterator iter = boundActors.begin(); iter != boundActors.end(); ++iter)
    {
        actors->push_back(*iter);
    }
}

void ActorManager::RemoveActor(Token actorId)
{
    if (!m_actors.Remove(actorId))
    {
        TRACE_ERROR(boost::format("Cannot remove actor with id %d") % actorId);
    }
}

void ActorManager::ClearActors()
{
    m_actors.Clear();
}

void ActorManager::UpdateActorPosition(Token actorId, const Vector2i& positionOld, const Vector2i& positionNew)
{
    const boost::shared_ptr<Actor> actor = GetActor(actorId).lock();
    if (actor && !m_actors.Update(actor->GetId(), positionNew))
    {
        TRACE_ERROR(
            boost::format("Cannot update position for actor %s with id %d from (%f, %f) to (%f, %f)") % actor->GetAlias() % actorId % positionOld.x % positionOld.y % positionNew.x % positionNew.y
        );
    }
}

void ActorManager::UpdateActorId(Token actorIdOld, Token actorIdNew)
{
    if (actorIdOld == actorIdNew)
    {
        return;
    }

    do
    {
        const boost::shared_ptr<Actor>* const actor = m_actors.Get(actorIdOld);
        if (actor == NULL)
        {
            break;
        }

        const boost::shared_ptr<Actor> temp = *actor;

        if (!m_actors.Remove(actorIdOld) || !m_actors.Add(temp, actorIdNew, temp->GetPosition()))
        {
            break;
        }

        return;
    }
    while (false);

    TRACE_ERROR(boost::format("Cannot change actor id from %d to %d") % actorIdOld % actorIdNew);
}

void ActorManager::ShowLayer(ActorLayer layer)
{
    m_layers |= BIT(layer);
}

void ActorManager::HideLayer(ActorLayer layer)
{
    m_layers &= ~BIT(layer);
}

void ActorManager::ShowAllLayers()
{
    m_layers = static_cast<unsigned>(-1);
}

void ActorManager::HideAllLayers()
{
    m_layers = 0;
}

void ActorManager::ShowShapes()
{
    m_renderShapes = true;
}

void ActorManager::HideShapes()
{
    m_renderShapes = false;
}

bool ActorManager::AreShapesVisible() const
{
    return m_renderShapes;
}

bool ActorManager::IsLayerVisible(ActorLayer layer) const
{
    return IS_TRUE(m_layers & BIT(layer));
}

bool ActorManager::ActorSorter(const boost::shared_ptr<Actor>& actor1, const boost::shared_ptr<Actor>& actor2)
{
    return actor1->GetLayer() < actor2->GetLayer() || (actor1->GetLayer() == actor2->GetLayer() && actor1->GetPosition().y < actor2->GetPosition().y);
}
