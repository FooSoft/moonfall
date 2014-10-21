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

class ActorManager
{
public:
    ActorManager();

    void                                    Render(const boost::shared_ptr<Surface>& target, const Rect4i& bounds);
    void                                    Update(float elapsed, const Rect4i& bounds);

    boost::weak_ptr<Actor>                  AddActor(const char* actorAlias, const Vector2i& actorPosition, const char* actorName);
    boost::weak_ptr<const Actor>            GetActor(Token actorId) const;
    boost::weak_ptr<Actor>                  GetActor(Token actorId);
    boost::weak_ptr<const Actor>            GetActor(const char* actorName) const;
    boost::weak_ptr<Actor>                  GetActor(const char* actorName);
    boost::weak_ptr<const Actor>            PickActor(const Vector2i& position, const Vector2i& extents = Vector2i(512, 512)) const;
    boost::weak_ptr<Actor>                  PickActor(const Vector2i& position, const Vector2i& extents = Vector2i(512, 512));
    void                                    EnumerateActors(std::vector<boost::weak_ptr<Actor> >* actors, const Vector2i& position, const Vector2i& extents = Vector2i(512, 512));
    void                                    EnumerateActors(std::vector<boost::weak_ptr<Actor> >* actors);
    void                                    RemoveActor(Token actorId);
    void                                    ClearActors();

    void                                    UpdateActorPosition(Token actorId, const Vector2i& positionOld, const Vector2i& positionNew);
    void                                    UpdateActorId(Token actorIdOld, Token actorIdNew);

    bool                                    IsLayerVisible(ActorLayer layer) const;
    void                                    ShowLayer(ActorLayer layer);
    void                                    HideLayer(ActorLayer layer);
    void                                    ShowAllLayers();
    void                                    HideAllLayers();

    void                                    ShowShapes();
    void                                    HideShapes();
    bool                                    AreShapesVisible() const;

private:
    static bool                             ActorSorter(const boost::shared_ptr<Actor>& actor1, const boost::shared_ptr<Actor>& actor2);

    Grid<boost::shared_ptr<Actor>, Token>   m_actors;
    std::vector<boost::shared_ptr<Actor> >  m_actorCache;
    unsigned                                m_layers;
    bool                                    m_renderShapes;
};
