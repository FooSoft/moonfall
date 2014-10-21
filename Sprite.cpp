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
#include "Sprite.h"
#include "Surface.h"

Sprite::Sprite(boost::shared_ptr<const Surface> surface, const Vector2i& position, const Vector2i& size, const ActorShape shapes[]) :
        m_surface(surface),
        m_position(position),
        m_size(size)
{
    memcpy(m_shapes, shapes, sizeof(m_shapes));
}

void Sprite::Render(const boost::shared_ptr<Surface>& target)
{
    m_surface->Blit(target, m_position, m_size);
}

Vector2i Sprite::GetSize() const
{
    return m_size;
}

const ActorShape* Sprite::GetShapes() const
{
    return m_shapes;
}
