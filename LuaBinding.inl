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

template <LuaBinding::LuaFunction T>
void LuaBinding::RegisterFunction(const char* name)
{
    lua_register(m_state, name, FunctionRouter<T>);
}

template <LuaBinding::LuaFunction T>
int LuaBinding::FunctionRouter(lua_State* state)
{
    lua_getfield(state, LUA_REGISTRYINDEX, "LuaBinding");
    const int top = lua_gettop(state);

    ASSERT(lua_isuserdata(state, top));
    LuaBinding* const binding = static_cast<LuaBinding*>(lua_touserdata(state, top));

    const int result = (binding->*T)();
    switch (result)
    {
    case ERROR_TYPE_PARAMETER:
        binding->DoError("Binding parameter error");
        return 0;
    case ERROR_TYPE_STATE:
        binding->DoError("Binding state error");
        return 0;
    default:
        return result;
    }
}

template <typename T>
void LuaBinding::RegisterConstant(const char* name, const T& value)
{
    Push(value);
    lua_setglobal(m_state, name);
}

template <typename T>
void LuaBinding::Push(const Vector2<T>& value)
{
    lua_newtable(m_state);
    const int top = lua_gettop(m_state);

    Push(value.x);
    Push(value.y);

    lua_setfield(m_state, top, "y");
    lua_setfield(m_state, top, "x");
}

template <typename T>
void LuaBinding::Push(const Rect4<T>& value)
{
    lua_newtable(m_state);
    const int top = lua_gettop(m_state);

    Push(value.x0);
    Push(value.y0);
    Push(value.x1);
    Push(value.y1);

    lua_setfield(m_state, top, "y1");
    lua_setfield(m_state, top, "x1");
    lua_setfield(m_state, top, "y0");
    lua_setfield(m_state, top, "x0");
}

template <typename T>
void LuaBinding::Push(const std::vector<T>& value)
{
    lua_newtable(m_state);
    const int top = lua_gettop(m_state);

    for (size_t i = 0; i < value.size(); ++i)
    {
        Push(static_cast<int>(i) + 1);
        Push(value[i]);
        lua_settable(m_state, top);
    }
}

template <typename K, typename V>
void LuaBinding::Push(const std::map<K, V>& value)
{
    lua_newtable(m_state);
    const int top = lua_gettop(m_state);

    for (typename std::map<K, V>::const_iterator iter = value.begin(); iter != value.end(); ++iter)
    {
        Push(iter->first);
        Push(iter->second);
        lua_settable(m_state, top);
    }
}

template <typename T>
bool LuaBinding::Peek(int index, Vector2<T>* value, const Vector2<T>& def)
{
    if (!lua_istable(m_state, index))
    {
        *value = def;
        return false;
    }

    lua_getfield(m_state, index, "x");
    lua_getfield(m_state, index, "y");

    const int top = lua_gettop(m_state);
    const bool success = Peek(top - 1, &value->x) & Peek(top - 0, &value->y);

    lua_pop(m_state, 2);

    if (!success)
    {
        *value = def;
        return false;
    }

    return true;
}

template <typename K, typename V>
bool LuaBinding::Peek(int index, std::map<K, V>* value)
{
    if (!lua_istable(m_state, index))
    {
        return false;
    }

    const int top = lua_gettop(m_state);

    PushNil();

    while (lua_next(m_state, index) != 0)
    {
        K mapKey;
        V mapValue;

        if (!Peek(top + 1, &mapKey) || !Peek(top + 2, &mapValue))
        {
            Pop(2);
            return false;
        }

        value->insert(std::make_pair(mapKey, mapValue));
        Pop(1);
    }

    Pop(1);
    return true;
}

template <typename T>
boost::shared_ptr<T> LuaBinding::GetActorProperty(Token actorId)
{
    boost::shared_ptr<Actor> actor = GetActor(actorId);
    return actor ? actor->GetProperty<T>().lock() : boost::shared_ptr<T>();
}

template <typename T>
boost::shared_ptr<T> LuaBinding::GetActorProperty(const char* actorName)
{
    boost::shared_ptr<Actor> actor = GetActor(actorName);
    return actor ? actor->GetProperty<T>().lock() : boost::shared_ptr<T>();
}
