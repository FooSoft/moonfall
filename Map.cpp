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
#include "Map.h"
#include "Animation.h"
#include "Sprite.h"

Map::Map() :
        m_tileSize(0)
{
}

void Map::MapTileAnimation::Render(const boost::shared_ptr<Surface>& target)
{
    m_animation->Render(target);
}

void Map::MapTileAnimation::Update(float elapsed)
{
    m_animation->Update(elapsed);
}

const ActorShape* Map::MapTileAnimation::GetShapes() const
{
    return m_animation->GetShapes();
}

void Map::MapTileSprite::Render(const boost::shared_ptr<Surface>& target)
{
    m_sprite->Render(target);
}

const ActorShape* Map::MapTileSprite::GetShapes() const
{
    return m_sprite->GetShapes();
}
