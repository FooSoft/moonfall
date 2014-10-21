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
#include "World.h"
#include "Surface.h"
#include "Sprite.h"
#include "WorldScript.h"

World::World(const char* alias) :
        m_alias(alias),
        m_backdropSpriteVisible(true),
        m_scriptQueuedCreate(false)
{
}

World::~World()
{
    SetScriptAlias(NULL);
}

void World::Render(const boost::shared_ptr<Surface>& target, const Rect4i& viewport)
{
    if (m_backdropSpriteVisible && m_backdropSprite)
    {
        const Rect4i bounds = m_camera.Frame(viewport);
        const Vector2i size = m_backdropSprite->GetSize();

        for (int y = bounds.y0 - size.y; y < bounds.y1 + size.y; y += size.y)
        {
            for (int x = bounds.x0 - size.x; x < bounds.x1 + size.x; x += size.x)
            {
                target->SetTranslation(Vector2i(x - bounds.x0 - bounds.x0 % size.x, y - bounds.y0 - bounds.y0 % size.y));
                m_backdropSprite->Render(target);
            }
        }
    }

    m_actorManager.Render(target, m_camera.Frame(viewport));
}

void World::Update(float elapsed, const Rect4i& viewport)
{
    m_actorManager.Update(elapsed, m_camera.Frame(viewport));

    if (m_script)
    {
        if (m_scriptQueuedCreate)
        {
            m_script->OnWorldCreate();
            m_scriptQueuedCreate = false;
        }

        m_script->OnWorldUpdate(elapsed);
    }

    m_camera.Update(elapsed);
}

bool World::SetBackdropSpriteAlias(const char* alias)
{
    m_backdropSprite = System::LoadSprite(alias);
    m_backdropSpriteAlias = alias;
    return m_backdropSprite;
}

const char* World::GetBackdropSpriteAlias() const
{
    return m_backdropSpriteAlias.c_str();
}

void World::ShowBackdropSprite(bool show)
{
    m_backdropSpriteVisible = show;
}

void World::HideBackdropSprite()
{
    ShowBackdropSprite(false);
}

bool World::IsBackdropSpriteVisible() const
{
    return m_backdropSpriteVisible;
}

bool World::SetScriptAlias(const char* alias)
{
    if (m_script)
    {
        m_script->OnWorldDestroy();
    }

    if (alias == NULL || *alias == '\0')
    {
        m_script.reset();
        m_scriptAlias = "";
    }
    else
    {
        m_script = System::LoadWorldScript(alias);
        m_scriptAlias = alias;
    }

    m_scriptQueuedCreate = m_script;
    return m_script;
}

const char* World::GetScriptAlias() const
{
    return m_scriptAlias.c_str();
}

const char* World::GetAlias() const
{
    return m_alias.c_str();
}

void World::SetScriptParameter(const char* name, const ScriptParameter& parameter)
{
    if (m_script)
    {
        m_script->SetParameter(name, parameter);
    }
}

bool World::GetScriptParameter(const char* name, ScriptParameter* parameter) const
{
    return m_script && m_script->GetParameter(name, parameter);
}

ActorManager* World::GetActorManager()
{
    return &m_actorManager;
}

Camera* World::GetCamera()
{
    return &m_camera;
}
