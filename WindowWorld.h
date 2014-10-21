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
#include "Window.h"
#include "InputManager.h"
#include "IWorldContext.h"

class WindowDialogue;

class WindowWorld : public Window, public IWorldContext
{
public:
    typedef                             std::list<IWorldListener*> ListenerList;

    WindowWorld(const char* worldAlias, const ParameterMap& parameters = ParameterMap(), const ListenerList& listeners = ListenerList());

    void                                LoadWorld(const char* worldAlias, const ParameterMap& parameters = ParameterMap());
    bool                                SaveWorld(const char* worldAlias);
    const char*                         GetWorldAlias() const;

    void                                AddListener(IWorldListener* listener);
    void                                RemoveListener(IWorldListener* listener);

    boost::weak_ptr<Actor>              AddActor(const char* actorAlias, const Vector2i& actorPosition, const char* actorName = NULL);
    boost::weak_ptr<const Actor>        GetActor(Token actorId) const;
    boost::weak_ptr<Actor>              GetActor(Token actorId);
    boost::weak_ptr<const Actor>        GetActor(const char* actorName) const;
    boost::weak_ptr<Actor>              GetActor(const char* actorName);
    boost::weak_ptr<const Actor>        PickActor(const Vector2i& position) const;
    boost::weak_ptr<Actor>              PickActor(const Vector2i& position);
    void                                RemoveActor(Token actorId);
    void                                ClearActors();

    bool                                IsLayerVisible(ActorLayer layer) const;
    void                                ShowLayer(ActorLayer layer);
    void                                HideLayer(ActorLayer layer);
    void                                ShowAllLayers();
    void                                HideAllLayers();

    const char*                         GetBackdropSpriteAlias() const;
    void                                SetBackdropSpriteAlias(const char* alias);
    void                                ShowBackdropSprite();
    void                                HideBackdropSprite();
    bool                                IsBackdropSpriteVisible() const;

    void                                SetScriptAlias(const char* alias);
    const char*                         GetScriptAlias() const;

    void                                ShowShapes();
    void                                HideShapes();
    bool                                AreShapesVisible() const;

    void                                SetCameraTarget(boost::shared_ptr<ICameraTarget> target);
    void                                SetCameraTarget(const Vector2i& target);
    Vector2i                            GetCameraTarget() const;
    Rect4i                              GetCameraBounds() const;

    bool                                IsKeyPressed(MetaKey key) const;
    bool                                IsKeyTriggered(MetaKey key) const;

    void                                BeginDialogue(const Rect4i& padding = Rect4i(125, 425, 125, 25));
    void                                EndDialogue();

private:
    void                                OnRender(const boost::shared_ptr<Surface>& target);
    void                                OnUpdate(float elapsed);
    void                                OnKeyDown(const InputKey& args);
    void                                OnKeyUp(const InputKey& args);
    void                                OnMouseMove(const InputMouse& args);
    void                                OnMouseButtonDown(const InputMouse& args);
    void                                OnMouseButtonUp(const InputMouse& args);
    void                                OnMouseWheel(const InputMouse& args);
    void                                OnActivate();
    void                                OnDeactivate();
    void                                OnLoad();
    void                                OnUnload();

    ListenerList                        m_listeners;
    boost::shared_ptr<World>            m_world;
    boost::shared_ptr<WindowDialogue>   m_dialogue;
    InputManager                        m_inputManager;
};
