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
#include "Actor.h"

class Sprite : private boost::noncopyable
{
public:
    Sprite(boost::shared_ptr<const Surface> surface, const Vector2i& position, const Vector2i& size, const ActorShape shapes[]);

    void                                Render(const boost::shared_ptr<Surface>& target);

    Vector2i                            GetSize() const;
    const ActorShape*                   GetShapes() const;

private:
    boost::shared_ptr<const Surface>    m_surface;
    ActorShape                          m_shapes[ACTOR_SHAPE_TYPE_COUNT];
    Vector2i                            m_position;
    Vector2i                            m_size;
};
