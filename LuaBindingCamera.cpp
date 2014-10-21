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
#include "LuaBinding.h"
#include "IWorldContext.h"
#include "CameraTarget.h"

int LuaBinding::CameraSetTargetStatic()
{
    Vector2i target;
    if (!Peek(1, &target))
    {
        return ERROR_TYPE_PARAMETER;
    }

    IWorldContext* const worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    worldContext->SetCameraTarget(target);
    return 0;
}

int LuaBinding::CameraSetTargetActor()
{
    int actorId = 0;
    if (!Peek(1, &actorId))
    {
        return ERROR_TYPE_PARAMETER;
    }

    IWorldContext* const worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    boost::shared_ptr<Actor> target = LuaBinding::GetActor(actorId);
    if (!target)
    {
        return ERROR_TYPE_STATE;
    }

    worldContext->SetCameraTarget(boost::shared_ptr<ICameraTarget>(new CameraTargetActor(target)));
    return 0;
}

int LuaBinding::CameraGetTarget()
{
    IWorldContext* const worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    Push(worldContext->GetCameraTarget());
    return 1;
}

int LuaBinding::CameraGetBounds()
{
    IWorldContext* const worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    Push(worldContext->GetCameraBounds());
    return 1;
}
