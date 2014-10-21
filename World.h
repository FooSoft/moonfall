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
#include "ActorManager.h"
#include "Camera.h"
#include "Map.h"

struct World
{
public:
    World(const char* alias);
    ~World();

    void                            Render(const boost::shared_ptr<Surface>& target, const Rect4i& viewport);
    void                            Update(float elapsed, const Rect4i& viewport);

    bool                            SetBackdropSpriteAlias(const char* alias);
    const char*                     GetBackdropSpriteAlias() const;
    void                            ShowBackdropSprite(bool show = true);
    void                            HideBackdropSprite();
    bool                            IsBackdropSpriteVisible() const;

    bool                            SetScriptAlias(const char* alias);
    const char*                     GetScriptAlias() const;
    void                            SetScriptParameter(const char* name, const ScriptParameter& parameter);
    bool                            GetScriptParameter(const char* name, ScriptParameter* parameter) const;

    const char*                     GetAlias() const;
    ActorManager*                   GetActorManager();
    Camera*                         GetCamera();

private:
    std::string                     m_alias;
    ActorManager                    m_actorManager;
    Camera                          m_camera;
    Map                             m_map;

    boost::shared_ptr<Sprite>       m_backdropSprite;
    std::string                     m_backdropSpriteAlias;
    bool                            m_backdropSpriteVisible;

    boost::shared_ptr<WorldScript>  m_script;
    std::string                     m_scriptAlias;
    bool                            m_scriptQueuedCreate;
};
