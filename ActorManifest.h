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

class ActorManifest
{
public:
    bool LoadManifest(const char* filename);
    void ClearManifest();

    boost::shared_ptr<Actor>    CreateActor(const char* actorAlias, const char* actorName, unsigned allowedProperties) const;
    void                        SummarizeActors(std::vector<ActorSummary>* summary) const;
    bool                        SummarizeActor(const char* actorAlias, ActorSummary* summary) const;

private:
    struct ActorEntry
    {
        ActorEntry() :
                properties(0), dynamic(false), layer(ACTOR_LAYER_GROUND1) { }

        struct
        {
            std::string         resource;
            Vector2i            offset;
        } sprite;

        struct
        {
            std::string         resource;
            Vector2i            offset;
            bool                play;
            bool                loop;
        } animation;

        struct
        {
            Vector2f            velocity;
        } physics;

        struct
        {
            std::string         resource;
        } script;

        unsigned                properties;
        std::string             thumbnail;
        bool                    dynamic;
        ActorLayer              layer;
    };

    void                        ParsePropertyElement(const TiXmlElement* element, ActorEntry* actorEntry);
    void                        ParsePropertyElementAnimation(const TiXmlElement* element, ActorEntry* actorEntry);
    void                        ParsePropertyElementSprite(const TiXmlElement* element, ActorEntry* actorEntry);
    void                        ParsePropertyElementPhysics(const TiXmlElement* element, ActorEntry* actorEntry);
    void                        ParsePropertyElementScript(const TiXmlElement* element, ActorEntry* actorEntry);

    typedef                     std::map<std::string, ActorEntry> ActorMap;
    ActorMap                    m_manifest;
};
