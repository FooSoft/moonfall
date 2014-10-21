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
#include "Camera.h"
#include "ICameraTarget.h"

void Camera::SetTarget(const boost::shared_ptr<ICameraTarget>& target)
{
    m_targetDynamic = target;
    m_targetDynamic->GetTarget(&m_targetStatic);
}

void Camera::SetTarget(const Vector2i& target)
{
    m_targetDynamic.reset();
    m_targetStatic = target;
}

Vector2i Camera::GetTarget() const
{
    return m_targetStatic;
}

void Camera::Update(float elapsed)
{
    if (m_targetDynamic)
    {
        m_targetDynamic->GetTarget(&m_targetStatic);
    }
}

Rect4i Camera::Frame(const Rect4i& viewport) const
{
    const Vector2i viewportExtents(viewport.GetWidth() / 2, viewport.GetHeight() / 2);
    return Rect4i(
               m_targetStatic.x - viewportExtents.x,
               m_targetStatic.y - viewportExtents.y,
               m_targetStatic.x + viewportExtents.x,
               m_targetStatic.y + viewportExtents.y
           );
}

Vector2i Camera::Project(const Rect4i& viewport, const Vector2i& point) const
{
    const Rect4i frame(Frame(viewport));
    return Vector2i(point.x + frame.x0, point.y + frame.y0);
}
