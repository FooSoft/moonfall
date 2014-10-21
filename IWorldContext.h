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

class ICameraTarget;

class IWorldListener
{
public:
    virtual void OnWorldKeyDown(const InputKey& args) = 0;
    virtual void OnWorldKeyUp(const InputKey& args) = 0;
    virtual void OnWorldMouseMove(const InputMouse& args) = 0;
    virtual void OnWorldMouseButtonDown(const InputMouse& args) = 0;
    virtual void OnWorldMouseButtonUp(const InputMouse& args) = 0;
    virtual void OnWorldMouseWheel(const InputMouse& args) = 0;
    virtual void OnWorldUpdate(float elapsed) = 0;
    virtual void OnWorldActivate() = 0;
    virtual void OnWorldDeactivate() = 0;

protected:
    ~IWorldListener() {}
};

class IWorldContext
{
public:
    virtual void                            AddListener(IWorldListener* listener) = 0;
    virtual void                            RemoveListener(IWorldListener* listener) = 0;

    virtual void                            LoadWorld(const char* worldAlias, const ParameterMap& parameters = ParameterMap()) = 0;
    virtual bool                            SaveWorld(const char* worldAlias) = 0;
    virtual const char*                     GetWorldAlias() const = 0;

    virtual boost::weak_ptr<Actor>          AddActor(const char* actorAlias, const Vector2i& actorPosition, const char* actorName = NULL) = 0;
    virtual boost::weak_ptr<const Actor>    GetActor(Token actorId) const = 0;
    virtual boost::weak_ptr<Actor>          GetActor(Token actorId) = 0;
    virtual boost::weak_ptr<const Actor>    GetActor(const char* actorName) const = 0;
    virtual boost::weak_ptr<Actor>          GetActor(const char* actorName) = 0;
    virtual boost::weak_ptr<const Actor>    PickActor(const Vector2i& position) const = 0;
    virtual boost::weak_ptr<Actor>          PickActor(const Vector2i& position) = 0;
    virtual void                            RemoveActor(Token actorId) = 0;
    virtual void                            ClearActors() = 0;

    virtual bool                            IsLayerVisible(ActorLayer layer) const = 0;
    virtual void                            ShowLayer(ActorLayer layer) = 0;
    virtual void                            HideLayer(ActorLayer layer) = 0;
    virtual void                            ShowAllLayers() = 0;
    virtual void                            HideAllLayers() = 0;

    virtual const char*                     GetBackdropSpriteAlias() const = 0;
    virtual void                            SetBackdropSpriteAlias(const char* alias) = 0;
    virtual void                            ShowBackdropSprite() = 0;
    virtual void                            HideBackdropSprite() = 0;
    virtual bool                            IsBackdropSpriteVisible() const = 0;

    virtual void                            SetScriptAlias(const char* alias) = 0;
    virtual const char*                     GetScriptAlias() const = 0;

    virtual void                            ShowShapes() = 0;
    virtual void                            HideShapes() = 0;
    virtual bool                            AreShapesVisible() const = 0;

    virtual void                            SetCameraTarget(boost::shared_ptr<ICameraTarget> target) = 0;
    virtual void                            SetCameraTarget(const Vector2i& target) = 0;
    virtual Vector2i                        GetCameraTarget() const = 0;
    virtual Rect4i                          GetCameraBounds() const = 0;

    virtual bool                            IsKeyPressed(MetaKey key) const = 0;
    virtual bool                            IsKeyTriggered(MetaKey key) const = 0;

protected:
    ~IWorldContext() {}
};
