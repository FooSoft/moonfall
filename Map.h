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

class Map
{
public:
    Map();


private:
    class IMapTile
    {
    public:
        virtual ~IMapTile() { }
        virtual void                    Render(const boost::shared_ptr<Surface>& target) = 0;
        virtual void                    Update(float elapsed) = 0;
        virtual const ActorShape*       GetShapes() const = 0;
    };

    class MapTileAnimation : public IMapTile
    {
    public:
        MapTileAnimation(const boost::shared_ptr<Animation>& animation) :
                m_animation(animation) { }

        void                            Render(const boost::shared_ptr<Surface>& target);
        void                            Update(float elapsed);
        const ActorShape*               GetShapes() const;

    private:
        boost::shared_ptr<Animation>    m_animation;
    };

    class MapTileSprite : public IMapTile
    {
    public:
        MapTileSprite(const boost::shared_ptr<Sprite>& sprite) :
                m_sprite(sprite) { }

        void                            Render(const boost::shared_ptr<Surface>& target);
        void                            Update(float) { }
        const ActorShape*               GetShapes() const;

    private:
        boost::shared_ptr<Sprite>       m_sprite;
    };

    int m_tileSize;
};
