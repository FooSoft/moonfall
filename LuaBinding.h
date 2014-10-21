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

#define LUA_DECLARE_FUNCTION(x) int x()

enum LuaType
{
    LUA_TYPE_NUMBER,
    LUA_TYPE_BOOLEAN,
    LUA_TYPE_STRING,
};

struct LuaValue
{
    LuaValue() :
            type(LUA_TYPE_STRING),
            valueNumber(0),
            valueBoolean(false) { }

    LuaType     type;
    std::string valueString;
    double      valueNumber;
    bool        valueBoolean;
};

class LuaBinding
{
public:
    LuaBinding();
    ~LuaBinding();

    bool                            DoString(const char* string);
    void                            ResetState();

    template <typename T>
    void                            Push(const Vector2<T>& value);
    template <typename T>
    void                            Push(const Rect4<T>& value);
    template <typename T>
    void                            Push(const std::vector<T>& value);
    template <typename K, typename V>
    void                            Push(const std::map<K, V>& value);
    void                            Push(const std::string& value);
    void                            Push(const char* value);
    void                            Push(double value);
    void                            Push(float value);
    void                            Push(int value);
    void                            Push(bool value);
    void                            Push(const ActorShape& value);
    void                            Push(const LuaValue& value);
    void                            Push(const ScriptParameter& value);
    void                            PushNil();

    template <typename T>
    bool                            Peek(int index, Vector2<T>* value, const Vector2<T>& def = Vector2<T>());
    template <typename K, typename V>
    bool                            Peek(int index, std::map<K, V>* value);
    bool                            Peek(int index, std::string* value, const std::string& def = "");
    bool                            Peek(int index, const char** value, const char* def = NULL);
    bool                            Peek(int index, double* value, double def = 0);
    bool                            Peek(int index, float* value, float def = 0);
    bool                            Peek(int index, int* value, int def = 0);
    bool                            Peek(int index, bool* value, bool def = false);
    bool                            Peek(int index, ActorShape* value, const ActorShape& def = ActorShape());
    bool                            Peek(int index, LuaValue* value, const LuaValue& def = LuaValue());
    bool                            Peek(int index, ScriptParameter* value, const ScriptParameter& def = ScriptParameter());
    bool                            PeekNil(int index);
    void                            Pop(int count);

    bool                            IsFunction(int index);
    void                            GetGlobal(const char* name);
    bool                            LoadString(const char* string);
    bool                            Call(int args, int results);
    void                            DoError(const char* error);
    int                             GetTop();

protected:
    enum
    {
        ERROR_TYPE_PARAMETER    = -1,
        ERROR_TYPE_STATE        = -2
    };

    typedef                         int (LuaBinding::*LuaFunction)();

    template <LuaFunction T>
    void                            RegisterFunction(const char* name);
    template <typename T>
    void                            RegisterConstant(const char* name, const T& value);

    template <typename T>
    static boost::shared_ptr<T>     GetActorProperty(Token actorId);
    template <typename T>
    static boost::shared_ptr<T>     GetActorProperty(const char* actorName);
    static boost::shared_ptr<Actor> GetActor(Token actorId);
    static boost::shared_ptr<Actor> GetActor(const char* name);

private:
    static bool                     LuaValueToScriptParameter(const LuaValue& value, ScriptParameter* parameter);
    static bool                     ScriptParameterToLuaValue(const ScriptParameter& parameter, LuaValue* value);

    template <LuaFunction T>
    static int                      FunctionRouter(lua_State* state);

    void                            TraceError(int result);
    void                            ExportFunctions();
    void                            ExportConstants();

    lua_State*                      m_state;

    // Util
    LUA_DECLARE_FUNCTION(UtilTraceInfo);
    LUA_DECLARE_FUNCTION(UtilTraceWarning);
    LUA_DECLARE_FUNCTION(UtilTraceError);
    LUA_DECLARE_FUNCTION(UtilScriptExecute);

    // Camera
    LUA_DECLARE_FUNCTION(CameraSetTargetStatic);
    LUA_DECLARE_FUNCTION(CameraSetTargetActor);
    LUA_DECLARE_FUNCTION(CameraGetTarget);
    LUA_DECLARE_FUNCTION(CameraGetBounds);

    // Actor
    LUA_DECLARE_FUNCTION(ActorAdd);
    LUA_DECLARE_FUNCTION(ActorRemove);
    LUA_DECLARE_FUNCTION(ActorGetPosition);
    LUA_DECLARE_FUNCTION(ActorSetPosition);
    LUA_DECLARE_FUNCTION(ActorGetLayer);
    LUA_DECLARE_FUNCTION(ActorSetLayer);
    LUA_DECLARE_FUNCTION(ActorValidate);
    LUA_DECLARE_FUNCTION(ActorFromName);
    LUA_DECLARE_FUNCTION(ActorGetName);
    LUA_DECLARE_FUNCTION(ActorGetAlias);
    LUA_DECLARE_FUNCTION(ActorHasProperty);
    LUA_DECLARE_FUNCTION(ActorGetShape);
    LUA_DECLARE_FUNCTION(ActorSetShape);
    LUA_DECLARE_FUNCTION(ActorEnableShape);
    LUA_DECLARE_FUNCTION(ActorDisableShape);
    LUA_DECLARE_FUNCTION(ActorIsShapeEnabled);

    // ActorPhysics
    LUA_DECLARE_FUNCTION(ActorPhysicsGetVelocity);
    LUA_DECLARE_FUNCTION(ActorPhysicsSetVelocity);
    LUA_DECLARE_FUNCTION(ActorPhysicsQueryShapeCollisions);

    // ActorSprite
    LUA_DECLARE_FUNCTION(ActorSpriteSet);
    LUA_DECLARE_FUNCTION(ActorSpriteGetSize);

    // ActorScript
    LUA_DECLARE_FUNCTION(ActorScriptSendMessage);

    // ActorAnimation
    LUA_DECLARE_FUNCTION(ActorAnimationSet);
    LUA_DECLARE_FUNCTION(ActorAnimationPlay);
    LUA_DECLARE_FUNCTION(ActorAnimationIsPlaying);
    LUA_DECLARE_FUNCTION(ActorAnimationStop);
    LUA_DECLARE_FUNCTION(ActorAnimationRewind);
    LUA_DECLARE_FUNCTION(ActorAnimationPause);
    LUA_DECLARE_FUNCTION(ActorAnimationGetSize);

    // Input
    LUA_DECLARE_FUNCTION(InputIsKeyPressed);
    LUA_DECLARE_FUNCTION(InputIsKeyTriggered);

    // World
    LUA_DECLARE_FUNCTION(WorldLoad);
};

#include "LuaBinding.inl"
