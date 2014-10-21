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

int LuaBinding::InputIsKeyPressed()
{
    int metaKey = META_KEY_COUNT;
    if (!Peek(1, &metaKey))
    {
        return ERROR_TYPE_PARAMETER;
    }

    IWorldContext * const worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    Push(worldContext->IsKeyPressed(static_cast<MetaKey>(metaKey)));
    return 1;
}

int LuaBinding::InputIsKeyTriggered()
{
    int metaKey = META_KEY_COUNT;
    if (!Peek(1, &metaKey))
    {
        return ERROR_TYPE_PARAMETER;
    }

    IWorldContext * const worldContext = System::QueryWorldContext();
    if (worldContext == NULL)
    {
        return ERROR_TYPE_STATE;
    }

    Push(worldContext->IsKeyTriggered(static_cast<MetaKey>(metaKey)));
    return 1;
}
