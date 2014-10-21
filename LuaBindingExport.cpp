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

#define LUA_EXPORT_CONSTANT(x) RegisterConstant(#x, x)
#define LUA_EXPORT_FUNCTION(x) RegisterFunction<&LuaBinding::x>(#x)

void LuaBinding::ExportFunctions()
{
    // Util
    LUA_EXPORT_FUNCTION(UtilTraceInfo);                     // UtilTraceInfo(text)
    LUA_EXPORT_FUNCTION(UtilTraceWarning);                  // UtilTraceWarning(text)
    LUA_EXPORT_FUNCTION(UtilTraceError);                    // UtilTraceError(text)
    LUA_EXPORT_FUNCTION(UtilScriptExecute);                 // UtilScriptExecute(script)

    // Camera
    LUA_EXPORT_FUNCTION(CameraSetTargetStatic);             // CameraSetTargetStatic(position)
    LUA_EXPORT_FUNCTION(CameraSetTargetActor);              // CameraSetTargetActor(id)
    LUA_EXPORT_FUNCTION(CameraGetTarget);                   // CameraGetTarget() => target
    LUA_EXPORT_FUNCTION(CameraGetBounds);                   // CameraGetBounds() => bounds

    // Actor
    LUA_EXPORT_FUNCTION(ActorAdd);                          // ActorAdd(alias, position, [name], [parameters]) => id
    LUA_EXPORT_FUNCTION(ActorRemove);                       // ActorRemove(id)
    LUA_EXPORT_FUNCTION(ActorValidate);                     // ActorValidate(id) => valid
    LUA_EXPORT_FUNCTION(ActorFromName);                     // ActorFromName(name) => id
    LUA_EXPORT_FUNCTION(ActorGetName);                      // ActorGetName(id) => name
    LUA_EXPORT_FUNCTION(ActorGetAlias);                     // ActorGetAlias(id) => alias
    LUA_EXPORT_FUNCTION(ActorGetPosition);                  // ActorGetPosition(id) => position
    LUA_EXPORT_FUNCTION(ActorSetPosition);                  // ActorSetPosition(id, position)
    LUA_EXPORT_FUNCTION(ActorGetLayer);                     // ActorGetLayer(id) => layer
    LUA_EXPORT_FUNCTION(ActorSetLayer);                     // ActorSetLayer(id, layer)
    LUA_EXPORT_FUNCTION(ActorHasProperty);                  // ActorHasProperty(id, property) => has
    LUA_EXPORT_FUNCTION(ActorGetShape);                     // ActorGetShape(id, type) => shape
    LUA_EXPORT_FUNCTION(ActorSetShape);                     // ActorSetShape(id, type, shape)
    LUA_EXPORT_FUNCTION(ActorEnableShape);                  // ActorEnableShape(id, type)
    LUA_EXPORT_FUNCTION(ActorDisableShape);                 // ActorDisableShape(id, type)
    LUA_EXPORT_FUNCTION(ActorIsShapeEnabled);               // ActorIsShapeEnabled(id, type) => enabled

    // ActorPhysics
    LUA_EXPORT_FUNCTION(ActorPhysicsGetVelocity);           // ActorPhysicsGetVelocity(id) => velocity
    LUA_EXPORT_FUNCTION(ActorPhysicsSetVelocity);           // ActorPhysicsSetVelocity(id, velocity)
    LUA_EXPORT_FUNCTION(ActorPhysicsQueryShapeCollisions);  // ActorPhysicsQueryShapeCollisions(id, type) => actors

    // ActorSprite
    LUA_EXPORT_FUNCTION(ActorSpriteSet);                    // ActorSpriteSet(resource)
    LUA_EXPORT_FUNCTION(ActorSpriteGetSize);                // ActorSpriteGetSize(id)

    // ActorScript
    LUA_EXPORT_FUNCTION(ActorScriptSendMessage);            // ActorScriptSendMessage(id, message, [parameters]) => response

    // ActorAnimation
    LUA_EXPORT_FUNCTION(ActorAnimationSet);                 // ActorAnimationSet(id, resource)
    LUA_EXPORT_FUNCTION(ActorAnimationPlay);                // ActorAnimationPlay(id, loop)
    LUA_EXPORT_FUNCTION(ActorAnimationIsPlaying);           // ActorAnimationIsPlaying(id) => playing
    LUA_EXPORT_FUNCTION(ActorAnimationStop);                // ActorAnimationStop(id)
    LUA_EXPORT_FUNCTION(ActorAnimationRewind);              // ActorAnimationRewind(id)
    LUA_EXPORT_FUNCTION(ActorAnimationPause);               // ActorAnimationPause(id)
    LUA_EXPORT_FUNCTION(ActorAnimationGetSize);             // ActorAnimationGetSize(id)

    // Input
    LUA_EXPORT_FUNCTION(InputIsKeyPressed);                 // InputIsKeyPressed(key) => pressed
    LUA_EXPORT_FUNCTION(InputIsKeyTriggered);               // InputIsKeyTriggered(key) => triggered

    // World
    LUA_EXPORT_FUNCTION(WorldLoad);                         // WorldLoad(alias, [parameters])
}

void LuaBinding::ExportConstants()
{
    // ActorPropertyType
    LUA_EXPORT_CONSTANT(ACTOR_PROPERTY_TYPE_ANIMATION);
    LUA_EXPORT_CONSTANT(ACTOR_PROPERTY_TYPE_SPRITE);
    LUA_EXPORT_CONSTANT(ACTOR_PROPERTY_TYPE_PHYSICS);
    LUA_EXPORT_CONSTANT(ACTOR_PROPERTY_TYPE_SCRIPT);

    // ActorShapeType
    LUA_EXPORT_CONSTANT(ACTOR_SHAPE_TYPE_COLLISION_SOLID);
    LUA_EXPORT_CONSTANT(ACTOR_SHAPE_TYPE_COLLISION_INTERACT);

    // ActorLayer
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_GROUND1);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_GROUND2);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_GROUND3);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_OBJECT1);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_OBJECT2);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_OBJECT3);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_SKY1);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_SKY2);
    LUA_EXPORT_CONSTANT(ACTOR_LAYER_SKY3);

    // MetaKey
    LUA_EXPORT_CONSTANT(META_KEY_UP);
    LUA_EXPORT_CONSTANT(META_KEY_RIGHT);
    LUA_EXPORT_CONSTANT(META_KEY_DOWN);
    LUA_EXPORT_CONSTANT(META_KEY_LEFT);
    LUA_EXPORT_CONSTANT(META_KEY_USE);
    LUA_EXPORT_CONSTANT(META_KEY_RUN);
}
