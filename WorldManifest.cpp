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
#include "WorldManifest.h"
#include "ActorPropertyScript.h"
#include "World.h"
#include "Actor.h"

bool WorldManifest::LoadManifest(const char* filename)
{
    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load world manifest %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse world manifest %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* worldElement = rootElement->FirstChildElement(); worldElement != NULL; worldElement = worldElement->NextSiblingElement())
    {
        if (strcmp(worldElement->Value(), "World") != 0)
        {
            continue;
        }

        const char* asset = worldElement->Attribute("asset");
        const char* alias = worldElement->Attribute("alias");

        if (asset == NULL || alias == NULL)
        {
            TRACE_WARNING("World definition is missing required fields");
            continue;
        }

        if (m_manifest.find(alias) != m_manifest.end())
        {
            TRACE_WARNING(boost::format("Duplicate world %s") % alias);
            continue;
        }

        m_manifest.insert(std::make_pair(alias, asset));
    }

    return true;
}

void WorldManifest::ClearManifest()
{
    m_manifest.clear();
}

void WorldManifest::SummarizeWorlds(std::vector<WorldSummary>* summary) const
{
    for (WorldMap::const_iterator iter = m_manifest.begin(); iter != m_manifest.end(); ++iter)
    {
        summary->push_back(WorldSummary(iter->first));
    }
}

boost::shared_ptr<World> WorldManifest::LoadWorld(const char* worldAlias, const ParameterMap& parameters) const
{
    WorldMap::const_iterator iter = m_manifest.find(worldAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load world %s") % worldAlias);
        return boost::shared_ptr<World>();
    }

    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(iter->second.c_str());
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load world from file %s") % iter->second.c_str());
        return boost::shared_ptr<World>();
    }

    Deserializer data(buffer.get());
    TRACE_INFO(boost::format("Loaded world %s") % worldAlias);

    WorldHeader worldHeader;
    if (!worldHeader.Deserialize(&data))
    {
        TRACE_ERROR(boost::format("Invalid world header data"));
        return boost::shared_ptr<World>();
    }

    boost::shared_ptr<World> world(new World(worldAlias));
    world->SetBackdropSpriteAlias(worldHeader.backdrop.c_str());
    world->SetScriptAlias(worldHeader.script.c_str());

    for (ParameterMap::const_iterator paramIter = parameters.begin(); paramIter != parameters.end(); ++paramIter)
    {
        world->SetScriptParameter(paramIter->first.c_str(), paramIter->second);
    }

    WorldActor worldActor;
    while (worldActor.Deserialize(&data))
    {
        const char* actorName = worldActor.name.empty() ? NULL : worldActor.name.c_str();
        const boost::shared_ptr<Actor> actor = world->GetActorManager()->AddActor(worldActor.alias.c_str(), worldActor.position, actorName).lock();

        WorldActorPropertyHeader worldPropertyHeader;
        while (worldPropertyHeader.Deserialize(&data))
        {
            if (worldPropertyHeader.type == ACTOR_PROPERTY_TYPE_SCRIPT)
            {
                WorldActorPropertyScript worldPropertyScript;
                if (!worldPropertyScript.Deserialize(&data))
                {
                    TRACE_ERROR(boost::format("Invalid world actor script property"));
                    return boost::shared_ptr<World>();
                }

                if (!actor)
                {
                    continue;
                }

                const boost::shared_ptr<ActorPropertyScript> propertyScript = actor->AddProperty<ActorPropertyScript>().lock();
                for (std::map<std::string, std::string>::iterator iter = worldPropertyScript.parameters.begin(); iter != worldPropertyScript.parameters.end(); ++iter)
                {
                    ScriptParameter parameter;
                    if (!propertyScript->GetParameter(iter->first.c_str(), &parameter))
                    {
                        TRACE_WARNING(boost::format("Attempted to set unsupported script parameter %s on actor %s, ignoring") % iter->first.c_str() % worldActor.alias.c_str());
                        continue;
                    }

                    propertyScript->SetParameter(iter->first.c_str(), ScriptParameter(iter->second.c_str(), parameter.type));
                }
            }
            else
            {
                TRACE_ERROR(boost::format("Invalid world actor property"));
                return boost::shared_ptr<World>();
            }
        }
    }

    return world;
}

bool WorldManifest::SaveWorld(const char* worldAlias, const boost::shared_ptr<World>& world) const
{
    WorldMap::const_iterator iter = m_manifest.find(worldAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot save world %s") % worldAlias);
        return false;
    }

    Buffer buffer;
    Serializer data(&buffer);

    WorldHeader worldHeader;
    worldHeader.backdrop = world->GetBackdropSpriteAlias();
    worldHeader.script = world->GetScriptAlias();
    worldHeader.Serialize(&data);

    std::vector<boost::weak_ptr<Actor> > actors;
    world->GetActorManager()->EnumerateActors(&actors);

    for (std::vector<boost::weak_ptr<Actor> >::iterator iter = actors.begin(); iter != actors.end(); ++iter)
    {
        const boost::shared_ptr<Actor> actor = iter->lock();
        if (actor && !actor->IsDynamic())
        {
            WorldActor worldActor;
            worldActor.alias = actor->GetAlias();
            worldActor.name = actor->GetName();
            worldActor.position = actor->GetPosition();
            worldActor.Serialize(&data);

            const boost::shared_ptr<ActorPropertyScript> propertyScript = actor->GetProperty<ActorPropertyScript>().lock();
            if (propertyScript)
            {
                WorldActorPropertyHeader(ACTOR_PROPERTY_TYPE_SCRIPT).Serialize(&data);

                std::vector<std::string> names;
                propertyScript->EnumerateParameters(&names);

                WorldActorPropertyScript worldPropertyScript;
                for (std::vector<std::string>::iterator iter = names.begin(); iter != names.end(); ++iter)
                {
                    ScriptParameter parameter;
                    if (propertyScript->GetParameter(iter->c_str(), &parameter))
                    {
                        worldPropertyScript.parameters.insert(std::make_pair(*iter, parameter.value));
                    }
                }

                worldPropertyScript.Serialize(&data);
            }

            WorldActorPropertyHeader(ACTOR_PROPERTY_TYPE_DELIMITER).Serialize(&data);
        }
    }

    return System::SaveFile(iter->second.c_str(), buffer);
}

bool WorldManifest::WorldHeader::Deserialize(Deserializer* data)
{
    int token = 0;
    int version = 0;
    return
        data->Read(&token) && token == WORLD_TOKEN &&
        data->Read(&version) && version == WORLD_VERSION &&
        data->ReadString(&script) &&
        data->ReadString(&backdrop);
}

void WorldManifest::WorldHeader::Serialize(Serializer* data) const
{
    data->Write(WORLD_TOKEN);
    data->Write(WORLD_VERSION);
    data->WriteString(script);
    data->WriteString(backdrop);
}

bool WorldManifest::WorldActor::Deserialize(Deserializer* data)
{
    return
        data->ReadString(&alias) &&
        data->ReadString(&name) &&
        data->Read(&position);
}

void WorldManifest::WorldActor::Serialize(Serializer* data) const
{
    data->WriteString(alias);
    data->WriteString(name);
    data->Write(position);
}

bool WorldManifest::WorldActorPropertyHeader::Deserialize(Deserializer* data)
{
    return data->Read(&type) && type != ACTOR_PROPERTY_TYPE_DELIMITER;
}

void WorldManifest::WorldActorPropertyHeader::Serialize(Serializer* data) const
{
    data->Write(type);
}

bool WorldManifest::WorldActorPropertyScript::Deserialize(Deserializer* data)
{
    parameters.clear();

    int count = 0;
    if (!data->Read(&count))
    {
        return false;
    }

    for (int i = 0; i < count; ++i)
    {
        std::string name;
        std::string value;
        if (!data->ReadString(&name) || !data->ReadString(&value))
        {
            return false;
        }

        parameters.insert(std::make_pair(name, value));
    }

    return true;
}

void WorldManifest::WorldActorPropertyScript::Serialize(Serializer* data) const
{
    data->Write(static_cast<int>(parameters.size()));

    for (ScriptMap::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter)
    {
        data->WriteString(iter->first);
        data->WriteString(iter->second);
    }
}
