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

LuaBinding::LuaBinding() :
        m_state(NULL)
{
    ResetState();
}

LuaBinding::~LuaBinding()
{
    lua_close(m_state);
}

void LuaBinding::ResetState()
{
    if (m_state != NULL)
    {
        lua_close(m_state);
    }

    m_state = luaL_newstate();
    luaL_openlibs(m_state);
    lua_pushlightuserdata(m_state, this);
    lua_setfield(m_state, LUA_REGISTRYINDEX, "LuaBinding");

    ExportFunctions();
    ExportConstants();
}

void LuaBinding::Push(const std::string& value)
{
    Push(value.c_str());
}

void LuaBinding::Push(const char* value)
{
    lua_pushstring(m_state, value);
}

void LuaBinding::Push(double value)
{
    lua_pushnumber(m_state, value);
}

void LuaBinding::Push(float value)
{
    lua_pushnumber(m_state, value);
}

void LuaBinding::Push(int value)
{
    lua_pushinteger(m_state, value);
}

void LuaBinding::Push(bool value)
{
    lua_pushboolean(m_state, value ? 1 : 0);
}

void LuaBinding::Push(const ActorShape& value)
{
    lua_newtable(m_state);
    const int top = lua_gettop(m_state);

    Push(value.data);
    Push(value.position);
    Push(value.size);
    Push(value.enabled);

    lua_setfield(m_state, top, "enabled");
    lua_setfield(m_state, top, "size");
    lua_setfield(m_state, top, "position");
    lua_setfield(m_state, top, "data");
}

void LuaBinding::Push(const LuaValue& value)
{
    switch (value.type)
    {
    case LUA_TYPE_BOOLEAN:
        Push(value.valueBoolean);
        break;
    case LUA_TYPE_NUMBER:
        Push(value.valueNumber);
        break;
    case LUA_TYPE_STRING:
        Push(value.valueString);
        break;
    }
}

void LuaBinding::PushNil()
{
    lua_pushnil(m_state);
}

void LuaBinding::Push(const ScriptParameter& value)
{
    LuaValue luaValue;
    if (ScriptParameterToLuaValue(value, &luaValue))
    {
        Push(luaValue);
    }
}

bool LuaBinding::Peek(int index, std::string* value, const std::string& def)
{
    const char* temp = NULL;
    if (!Peek(index, &temp))
    {
        *value = def;
        return false;
    }

    *value = temp;
    return true;
}

bool LuaBinding::Peek(int index, const char** value, const char* def)
{
    if (!lua_isstring(m_state, index))
    {
        *value = def;
        return false;
    }

    *value = lua_tostring(m_state, index);
    return true;
}

bool LuaBinding::Peek(int index, double* value, double def)
{
    if (!lua_isnumber(m_state, index))
    {
        *value = def;
        return false;
    }

    *value = lua_tonumber(m_state, index);
    return true;
}

bool LuaBinding::Peek(int index, float* value, float def)
{
    if (!lua_isnumber(m_state, index))
    {
        *value = def;
        return false;
    }

    *value = lua_tonumber(m_state, index);
    return true;
}

bool LuaBinding::Peek(int index, int* value, int def)
{
    if (!lua_isnumber(m_state, index))
    {
        *value = def;
        return false;
    }

    *value = lua_tointeger(m_state, index);
    return true;
}

bool LuaBinding::Peek(int index, bool* value, bool def)
{
    if (!lua_isboolean(m_state, index))
    {
        *value = def;
        return false;
    }

    *value = lua_toboolean(m_state, index) != 0;
    return true;
}

bool LuaBinding::Peek(int index, ActorShape* value, const ActorShape& def)
{
    if (!lua_istable(m_state, index))
    {
        *value = def;
        return false;
    }

    lua_getfield(m_state, index, "data");
    lua_getfield(m_state, index, "position");
    lua_getfield(m_state, index, "size");
    lua_getfield(m_state, index, "enabled");

    const int top = lua_gettop(m_state);
    const bool success = Peek(top - 3, &value->data) & Peek(top - 2, &value->position) & Peek(top - 1, &value->size) & Peek(top - 0, &value->enabled);

    lua_pop(m_state, 4);

    if (!success)
    {
        *value = def;
        return false;
    }

    return true;
}

bool LuaBinding::Peek(int index, LuaValue* value, const LuaValue& def)
{
    if (Peek(index, &value->valueBoolean))
    {
        value->type = LUA_TYPE_BOOLEAN;
        return true;
    }

    if (Peek(index, &value->valueNumber))
    {
        value->type = LUA_TYPE_NUMBER;
        return true;
    }

    if (Peek(index, &value->valueString))
    {
        value->type = LUA_TYPE_STRING;
        return true;
    }

    *value = def;
    return false;
}

bool LuaBinding::PeekNil(int index)
{
    return lua_isnil(m_state, index);
}

bool LuaBinding::Peek(int index, ScriptParameter* value, const ScriptParameter& def)
{
    LuaValue luaValue;
    if (!Peek(index, &luaValue))
    {
        *value = def;
        return false;
    }

    ScriptParameter parameter;
    if (!LuaValueToScriptParameter(luaValue, &parameter))
    {
        *value = def;
        return false;
    }

    *value = parameter;
    return true;
}

void LuaBinding::Pop(int count)
{
    lua_pop(m_state, count);
}

void LuaBinding::TraceError(int result)
{
    const int top = lua_gettop(m_state);
    const char* error = lua_tostring(m_state, top);

    if (error == NULL)
    {
        return;
    }

    switch (result)
    {
    case LUA_ERRSYNTAX:
        Trace::Write(boost::format("Syntax error: %s") % error, Trace::LEVEL_ERROR, "Lua");
        break;
    case LUA_ERRRUN:
        Trace::Write(boost::format("Runtime error: %s") % error, Trace::LEVEL_ERROR, "Lua");
        break;
    case LUA_ERRMEM:
        Trace::Write(boost::format("Memory allocation error: %s") % error, Trace::LEVEL_ERROR, "Lua");
        break;
    case LUA_ERRERR:
        Trace::Write(boost::format("Error handler function error: %s") % error, Trace::LEVEL_ERROR, "Lua");
        break;
    }
}

bool LuaBinding::IsFunction(int index)
{
    return lua_isfunction(m_state, index);
}

void LuaBinding::GetGlobal(const char* name)
{
    lua_getglobal(m_state, name);
}

bool LuaBinding::DoString(const char* string)
{
    return LoadString(string) && Call(0, LUA_MULTRET);
}

bool LuaBinding::Call(int args, int results)
{
    const int result = lua_pcall(m_state, args, results, 0);
    if (result != 0)
    {
        TraceError(result);
    }
    return result == 0;
}

bool LuaBinding::LoadString(const char* string)
{
    const int result = luaL_loadstring(m_state, string);
    if (result != 0)
    {
        TraceError(result);
    }
    return result == 0;
}

void LuaBinding::DoError(const char* string)
{
    luaL_error(m_state, string);
}

int LuaBinding::GetTop()
{
    return lua_gettop(m_state);
}

boost::shared_ptr<Actor> LuaBinding::GetActor(Token actorId)
{
    boost::shared_ptr<Actor> actor;
    IWorldContext* worldContext = System::QueryWorldContext();
    if (worldContext != NULL)
    {
        actor = worldContext->GetActor(actorId).lock();
    }
    return actor;
}

boost::shared_ptr<Actor> LuaBinding::GetActor(const char* actorName)
{
    boost::shared_ptr<Actor> actor;
    IWorldContext* worldContext = System::QueryWorldContext();
    if (worldContext != NULL)
    {
        actor = worldContext->GetActor(actorName).lock();
    }
    return actor;
}

bool LuaBinding::LuaValueToScriptParameter(const LuaValue& value, ScriptParameter* parameter)
{
    static char s_buffer[256] = {0};

    switch (value.type)
    {
    case LUA_TYPE_NUMBER:
        parameter->type = SCRIPT_PARAMETER_TYPE_NUMBER;
        sprintf(s_buffer, "%f", value.valueNumber);
        parameter->value = s_buffer;
        return true;
    case LUA_TYPE_BOOLEAN:
        parameter->type = SCRIPT_PARAMETER_TYPE_BOOLEAN;
        parameter->value = value.valueBoolean ? "true" : "false";
        return true;
    case LUA_TYPE_STRING:
        parameter->type = SCRIPT_PARAMETER_TYPE_STRING;
        parameter->value = value.valueString;
        return true;
    default:
        return false;
    }
}

bool LuaBinding::ScriptParameterToLuaValue(const ScriptParameter& parameter, LuaValue* value)
{
    switch (parameter.type)
    {
    case SCRIPT_PARAMETER_TYPE_NUMBER:
        value->type = LUA_TYPE_NUMBER;
        value->valueNumber = atof(parameter.value.c_str());
        return true;
    case SCRIPT_PARAMETER_TYPE_BOOLEAN:
        value->type = LUA_TYPE_BOOLEAN;
        value->valueBoolean = parameter.value == "true";
        return true;
    case SCRIPT_PARAMETER_TYPE_STRING:
        value->type = LUA_TYPE_STRING;
        value->valueString = parameter.value;
        return true;
    default:
        return false;
    }
}
