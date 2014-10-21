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

enum ActorPropertyType
{
    ACTOR_PROPERTY_TYPE_ANIMATION,
    ACTOR_PROPERTY_TYPE_SPRITE,
    ACTOR_PROPERTY_TYPE_PHYSICS,
    ACTOR_PROPERTY_TYPE_SCRIPT,

    ACTOR_PROPERTY_TYPE_COUNT,
    ACTOR_PROPERTY_TYPE_DELIMITER = 0xff
};

enum ActorLayer
{
    ACTOR_LAYER_GROUND1,
    ACTOR_LAYER_GROUND2,
    ACTOR_LAYER_GROUND3,
    ACTOR_LAYER_OBJECT1,
    ACTOR_LAYER_OBJECT2,
    ACTOR_LAYER_OBJECT3,
    ACTOR_LAYER_SKY1,
    ACTOR_LAYER_SKY2,
    ACTOR_LAYER_SKY3,

    ACTOR_LAYER_COUNT
};

enum ActorShapeType
{
    ACTOR_SHAPE_TYPE_COLLISION_SOLID,
    ACTOR_SHAPE_TYPE_COLLISION_INTERACT,

    ACTOR_SHAPE_TYPE_COUNT,

    ACTOR_SHAPE_TYPE_COLLISION_FIRST    = ACTOR_SHAPE_TYPE_COLLISION_SOLID,
    ACTOR_SHAPE_TYPE_COLLISION_LAST     = ACTOR_SHAPE_TYPE_COLLISION_INTERACT,
    ACTOR_SHAPE_TYPE_COLLISION_COUNT    = ACTOR_SHAPE_TYPE_COLLISION_LAST - ACTOR_SHAPE_TYPE_COLLISION_FIRST + 1,
};

enum MetaKey
{
    META_KEY_UP,
    META_KEY_RIGHT,
    META_KEY_DOWN,
    META_KEY_LEFT,
    META_KEY_USE,
    META_KEY_RUN,

    META_KEY_COUNT
};

enum ScriptParameterType
{
    SCRIPT_PARAMETER_TYPE_BOOLEAN,
    SCRIPT_PARAMETER_TYPE_STRING,
    SCRIPT_PARAMETER_TYPE_NUMBER,
};

class Surface;

struct ActorShape
{
    ActorShape(int data, const Vector2i& position, const Vector2i& size, bool enabled) :
            data(data), position(position), size(size), enabled(enabled) { }

    ActorShape() :
            data(0), enabled(false) { }

    int                         data;
    Vector2i                    position;
    Vector2i                    size;
    bool                        enabled;
};

struct WorldSummary
{
    WorldSummary(const std::string& alias) :
            alias(alias) { }

    std::string                 alias;
};

struct ActorSummary
{
    ActorSummary(const std::string& alias, unsigned properties, boost::shared_ptr<Surface> thumbnail, ActorLayer layer) :
            alias(alias), properties(properties), thumbnail(thumbnail), layer(layer) { }

    ActorSummary() :
            properties(0), layer(ACTOR_LAYER_GROUND1) { }

    std::string                 alias;
    unsigned                    properties;
    boost::shared_ptr<Surface>  thumbnail;
    ActorLayer                  layer;
};

struct SpriteSummary
{
    SpriteSummary(const std::string& alias, const std::string& asset, const Vector2i& position, const Vector2i& size, const ActorShape shapes[]) :
            alias(alias), asset(asset), position(position), size(size)
    {
        memcpy(this->shapes, shapes, sizeof(this->shapes));
    }

    ActorShape                  shapes[ACTOR_SHAPE_TYPE_COUNT];
    std::string                 alias;
    std::string                 asset;
    Vector2i                    position;
    Vector2i                    size;
};

struct ScriptParameter
{
    ScriptParameter(const std::string& value, ScriptParameterType type) :
            value(value), type(type) { }

    ScriptParameter() :
            type(SCRIPT_PARAMETER_TYPE_STRING) { }

    std::string                 value;
    ScriptParameterType         type;
};

typedef std::map<std::string, ScriptParameter>  ParameterMap;

struct ScriptSummary
{
    ScriptSummary(const std::string& alias, const std::string& asset, const ParameterMap& parameters) :
            alias(alias), asset(asset), parameters(parameters) { }

    std::string                 alias;
    std::string                 asset;
    ParameterMap                parameters;
};
