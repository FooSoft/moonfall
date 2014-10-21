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
#include "ActorManifest.h"
#include "Actor.h"
#include "ActorPropertyAnimation.h"
#include "ActorPropertyPhysics.h"
#include "ActorPropertyScript.h"
#include "ActorPropertySprite.h"
#include "Surface.h"
#include "Sprite.h"

bool ActorManifest::LoadManifest(const char* filename)
{
    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load actor manifest %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse actor manifest %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* actorElement = rootElement->FirstChildElement(); actorElement != NULL; actorElement = actorElement->NextSiblingElement())
    {
        if (strcmp(actorElement->Value(), "Actor") != 0)
        {
            continue;
        }

        const char* alias = actorElement->Attribute("alias");
        if (alias == NULL)
        {
            TRACE_WARNING("Actor definition is missing required fields");
            continue;
        }

        if (m_manifest.find(alias) != m_manifest.end())
        {
            TRACE_WARNING(boost::format("Duplicate actor %s") % alias);
            continue;
        }

        ActorEntry entry;

        const char* thumbnail = actorElement->Attribute("thumbnail");
        if (thumbnail != NULL)
        {
            entry.thumbnail = thumbnail;
        }

        int dynamic = 0;
        actorElement->QueryIntAttribute("dynamic", &dynamic);
        entry.dynamic = dynamic != 0;

        int layer = ACTOR_LAYER_GROUND1;
        actorElement->QueryIntAttribute("layer", &layer);
        if (layer < ACTOR_LAYER_COUNT)
        {
            entry.layer = static_cast<ActorLayer>(layer);
        }

        for (const TiXmlElement* childElement = actorElement->FirstChildElement(); childElement != NULL; childElement = childElement->NextSiblingElement())
        {
            if (strcmp(childElement->Value(), "Properties") == 0)
            {
                ParsePropertyElement(childElement, &entry);
            }
        }

        m_manifest.insert(std::make_pair(alias, entry));
    }

    return true;
}

void ActorManifest::ClearManifest()
{
    m_manifest.clear();
}

void ActorManifest::ParsePropertyElement(const TiXmlElement* element, ActorEntry* actorEntry)
{
    for (const TiXmlElement* propertyElement = element->FirstChildElement(); propertyElement != NULL; propertyElement = propertyElement->NextSiblingElement())
    {
        if (strcmp(propertyElement->Value(), "Animation") == 0)
        {
            ParsePropertyElementAnimation(propertyElement, actorEntry);
            actorEntry->properties |= BIT(ACTOR_PROPERTY_TYPE_ANIMATION);
        }
        else if (strcmp(propertyElement->Value(), "Sprite") == 0)
        {
            ParsePropertyElementSprite(propertyElement, actorEntry);
            actorEntry->properties |= BIT(ACTOR_PROPERTY_TYPE_SPRITE);
        }
        else if (strcmp(propertyElement->Value(), "Physics") == 0)
        {
            ParsePropertyElementPhysics(propertyElement, actorEntry);
            actorEntry->properties |= BIT(ACTOR_PROPERTY_TYPE_PHYSICS);
        }
        else if (strcmp(propertyElement->Value(), "Script") == 0)
        {
            ParsePropertyElementScript(propertyElement, actorEntry);
            actorEntry->properties |= BIT(ACTOR_PROPERTY_TYPE_SCRIPT);
        }
    }
}

void ActorManifest::ParsePropertyElementAnimation(const TiXmlElement* element, ActorEntry* actorEntry)
{
    int animationPlay = 0;
    element->QueryIntAttribute("play", &animationPlay);
    actorEntry->animation.play = animationPlay != 0;

    int animationLoop = 0;
    element->QueryIntAttribute("loop", &animationLoop);
    actorEntry->animation.loop = animationLoop != 0;

    const char* animationResource = element->Attribute("resource");
    if (animationResource != NULL)
    {
        actorEntry->animation.resource = animationResource;
    }
}

void ActorManifest::ParsePropertyElementSprite(const TiXmlElement* element, ActorEntry* actorEntry)
{
    const char* spriteResource = element->Attribute("resource");
    if (spriteResource != NULL)
    {
        actorEntry->sprite.resource = spriteResource;
    }
}

void ActorManifest::ParsePropertyElementPhysics(const TiXmlElement* element, ActorEntry* actorEntry)
{
    element->QueryFloatAttribute("velocityX", &actorEntry->physics.velocity.x);
    element->QueryFloatAttribute("velocityY", &actorEntry->physics.velocity.y);
}

void ActorManifest::ParsePropertyElementScript(const TiXmlElement* element, ActorEntry* actorEntry)
{
    const char* scriptResource = element->Attribute("resource");
    if (scriptResource != NULL)
    {
        actorEntry->script.resource = scriptResource;
    }
}

boost::shared_ptr<Actor> ActorManifest::CreateActor(const char* actorAlias, const char* actorName, unsigned allowedProperties) const
{
    ActorMap::const_iterator iter = m_manifest.find(actorAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot create actor %s") % actorAlias);
        return boost::shared_ptr<Actor>();
    }

    const ActorEntry& actorEntry = iter->second;
    boost::shared_ptr<Actor> actor(new Actor(actorAlias, actorName));

    actor->SetDynamic(actorEntry.dynamic);
    actor->SetLayer(actorEntry.layer);

    if (actorEntry.properties & BIT(ACTOR_PROPERTY_TYPE_ANIMATION) & allowedProperties)
    {
        boost::shared_ptr<ActorPropertyAnimation> property = actor->AddProperty<ActorPropertyAnimation>().lock();

        if (!actorEntry.animation.resource.empty())
        {
            property->SetAnimation(actorEntry.animation.resource.c_str());
        }

        if (actorEntry.animation.play)
        {
            property->PlayAnimation(actorEntry.animation.loop);
        }
    }

    if (actorEntry.properties & BIT(ACTOR_PROPERTY_TYPE_SPRITE) & allowedProperties)
    {
        boost::shared_ptr<ActorPropertySprite> property = actor->AddProperty<ActorPropertySprite>().lock();
        property->SetSprite(actorEntry.sprite.resource.c_str());
    }

    if (actorEntry.properties & BIT(ACTOR_PROPERTY_TYPE_PHYSICS) & allowedProperties)
    {
        boost::shared_ptr<ActorPropertyPhysics> property = actor->AddProperty<ActorPropertyPhysics>().lock();
        property->SetVelocity(actorEntry.physics.velocity);
    }

    if (actorEntry.properties & BIT(ACTOR_PROPERTY_TYPE_SCRIPT) & allowedProperties)
    {
        boost::shared_ptr<ActorPropertyScript> property = actor->AddProperty<ActorPropertyScript>().lock();
        property->SetScript(actorEntry.script.resource.c_str());
    }

    return actor;
}

void ActorManifest::SummarizeActors(std::vector<ActorSummary>* summary) const
{
    for (ActorMap::const_iterator iter = m_manifest.begin(); iter != m_manifest.end(); ++iter)
    {
        const std::string& actorAlias = iter->first;
        const ActorEntry& actorEntry = iter->second;

        boost::shared_ptr<Surface> actorSurface;
        if (!actorEntry.thumbnail.empty())
        {
            boost::shared_ptr<Sprite> actorSprite = System::LoadSprite(actorEntry.thumbnail.c_str());
            if (actorSprite)
            {
                actorSurface = System::CreateSurface(actorSprite->GetSize(), COLOR_MODE_RGB_888);
                actorSurface->Clear(Color4b(0x00, 0x00, 0xff, 0x00));
                actorSprite->Render(actorSurface);
            }
        }

        summary->push_back(ActorSummary(actorAlias, actorEntry.properties, actorSurface, actorEntry.layer));
    }
}

bool ActorManifest::SummarizeActor(const char* actorAlias, ActorSummary* summary) const
{
    ActorMap::const_iterator iter = m_manifest.find(actorAlias);
    if (iter == m_manifest.end())
    {
        return false;
    }

    const ActorEntry& actorEntry = iter->second;

    boost::shared_ptr<Surface> actorSurface;
    if (!actorEntry.thumbnail.empty())
    {
        boost::shared_ptr<Sprite> actorSprite = System::LoadSprite(actorEntry.thumbnail.c_str());
        if (actorSprite)
        {
            actorSurface = System::CreateSurface(actorSprite->GetSize(), COLOR_MODE_RGB_888);
            actorSurface->Clear(Color4b(0x00, 0x00, 0xff, 0x00));
            actorSprite->Render(actorSurface);
        }
    }

    *summary = ActorSummary(actorAlias, actorEntry.properties, actorSurface, actorEntry.layer);
    return true;
}
