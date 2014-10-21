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

class WorldManifest
{
public:
    bool LoadManifest(const char* filename);
    void ClearManifest();

    boost::shared_ptr<World>    LoadWorld(const char* worldAlias, const ParameterMap& parameters = ParameterMap()) const;
    bool                        SaveWorld(const char* worldAlias, const boost::shared_ptr<World>& world) const;
    void                        SummarizeWorlds(std::vector<WorldSummary>* summary) const;

private:
    typedef                     std::map<std::string, std::string> WorldMap;
    typedef                     std::map<std::string, std::string> ScriptMap;

    struct WorldHeader
    {
        enum WorldConstants
        {
            WORLD_TOKEN         = 'UUGU',
            WORLD_VERSION       = 0x00000000
        };

        bool                    Deserialize(Deserializer* data);
        void                    Serialize(Serializer* data) const;

        std::string             script;
        std::string             backdrop;
    };

    struct WorldActor
    {
        WorldActor() :
                position(0) { }

        bool                    Deserialize(Deserializer* data);
        void                    Serialize(Serializer* data) const;

        std::string             alias;
        std::string             name;
        Vector2i                position;
    };

    struct WorldActorPropertyHeader
    {
        WorldActorPropertyHeader(ActorPropertyType type) :
                type(type) { }

        WorldActorPropertyHeader() :
                type(ACTOR_PROPERTY_TYPE_DELIMITER) { }

        bool                    Deserialize(Deserializer* data);
        void                    Serialize(Serializer* data) const;

        ActorPropertyType       type;
    };

    struct WorldActorPropertyScript
    {
        bool                    Deserialize(Deserializer* data);
        void                    Serialize(Serializer* data) const;

        ScriptMap               parameters;
    };

    WorldMap                    m_manifest;
};
