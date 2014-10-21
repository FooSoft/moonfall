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

int LuaBinding::UtilTraceInfo()
{
    const char* text = NULL;
    if (!Peek(1, &text))
    {
        return ERROR_TYPE_PARAMETER;
    }

    Trace::Write(text, Trace::LEVEL_INFO, "Lua");
    return 0;
}

int LuaBinding::UtilTraceWarning()
{
    const char* text = NULL;
    if (!Peek(1, &text))
    {
        return ERROR_TYPE_PARAMETER;
    }

    Trace::Write(text, Trace::LEVEL_WARNING, "Lua");
    return 0;
}

int LuaBinding::UtilTraceError()
{
    const char* text = NULL;
    if (!Peek(1, &text))
    {
        return ERROR_TYPE_PARAMETER;
    }

    Trace::Write(text, Trace::LEVEL_ERROR, "Lua");
    return 0;
}

int LuaBinding::UtilScriptExecute()
{
    const char* script = NULL;
    if (!Peek(1, &script))
    {
        return ERROR_TYPE_PARAMETER;
    }

    boost::shared_ptr<const Buffer> code = System::LoadScriptCode(script);
    if (!code)
    {
        return ERROR_TYPE_STATE;
    }

    DoString(static_cast<const char*>(code->Get()));
    return 0;
}
