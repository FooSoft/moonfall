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
#include "CameraTarget.h"
#include "Actor.h"

CameraTargetActor::CameraTargetActor(const boost::weak_ptr<Actor>& target) :
        m_target(target)
{
}

bool CameraTargetActor::GetTarget(Vector2i* target) const
{
    boost::shared_ptr<Actor> actor = m_target.lock();
    if (actor)
    {
        *target = actor->GetPosition();
    }
    return actor;
}
