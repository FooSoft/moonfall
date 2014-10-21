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
#include "WindowWorld.h"
#include "WindowDialogue.h"
#include "Surface.h"
#include "Actor.h"
#include "World.h"

WindowWorld::WindowWorld(const char* worldAlias, const ParameterMap& parameters, const ListenerList& listeners) :
        m_listeners(listeners)
{
    m_world = System::LoadWorld(worldAlias, parameters);
}

void WindowWorld::OnRender(const boost::shared_ptr<Surface>& target)
{
    target->Clear(Color4b(0x00, 0x00, 0xff, 0x00));

    if (m_world)
    {
        target->PushState();
        m_world->Render(target, GetBounds());
        target->PopState();
    }

    Window::OnRender(target);
}

void WindowWorld::OnUpdate(float elapsed)
{
    Window::OnUpdate(elapsed);

    if (!m_world)
    {
        return;
    }

    m_world->Update(elapsed, GetBounds());

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldUpdate(elapsed);
    }

    m_inputManager.Update(elapsed);
}

void WindowWorld::LoadWorld(const char* worldAlias, const ParameterMap& parameters)
{
    System::EnqueueWindow(boost::shared_ptr<Window>(new WindowWorld(worldAlias, parameters, m_listeners)));
}

bool WindowWorld::SaveWorld(const char* worldAlias)
{
    return m_world && System::SaveWorld(worldAlias, m_world);
}

const char* WindowWorld::GetWorldAlias() const
{
    return m_world ? m_world->GetAlias() : NULL;
}

void WindowWorld::OnKeyDown(const InputKey& args)
{
    Window::OnKeyDown(args);

    if (!m_world)
    {
        return;
    }

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldKeyDown(args);
    }

    m_inputManager.DoKeyDown(args);
}

void WindowWorld::OnKeyUp(const InputKey& args)
{
    Window::OnKeyUp(args);

    if (!m_world)
    {
        return;
    }

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldKeyUp(args);
    }

    m_inputManager.DoKeyUp(args);
}

void WindowWorld::OnMouseMove(const InputMouse& args)
{
    Window::OnMouseMove(args);

    if (!m_world)
    {
        return;
    }

    const Vector2i position = m_world->GetCamera()->Project(GetBounds(), args.position);
    const Vector2i delta(args.delta.x, args.delta.y);

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldMouseMove(InputMouse(args.button, args.buttonState, position, delta, args.wheel, args.modifier));
    }
}

void WindowWorld::OnMouseButtonDown(const InputMouse& args)
{
    Window::OnMouseButtonDown(args);

    if (!m_world)
    {
        return;
    }

    const Vector2i position = m_world->GetCamera()->Project(GetBounds(), args.position);

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldMouseButtonDown(InputMouse(args.button, args.buttonState, position, Vector2i(0, 0), args.wheel, args.modifier));
    }

#ifdef DEVELOPER
    if (args.modifier & KMOD_SHIFT && args.button == SDL_BUTTON_LEFT)
    {
        boost::shared_ptr<Actor> actor = PickActor(position).lock();
        if (actor)
        {
            TRACE_INFO(boost::format("Actor:\n\tAlias = %s\n\tId = %d\n\tName = %s\n\tLayer = %d\n\tPosition = (%.1f, %.1f)") %
                       actor->GetAlias() % actor->GetId() % actor->GetName() % actor->GetLayer() % actor->GetPosition().x % actor->GetPosition().y);
        }
    }
#endif
}

void WindowWorld::OnMouseButtonUp(const InputMouse& args)
{
    Window::OnMouseButtonUp(args);

    if (!m_world)
    {
        return;
    }

    const Vector2i position = m_world->GetCamera()->Project(GetBounds(), args.position);

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldMouseButtonUp(InputMouse(args.button, args.buttonState, position, Vector2i(0, 0), args.wheel, args.modifier));
    }
}

void WindowWorld::OnMouseWheel(const InputMouse& args)
{
    Window::OnMouseWheel(args);

    if (!m_world)
    {
        return;
    }

    const Vector2i position = m_world->GetCamera()->Project(GetBounds(), args.position);

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldMouseWheel(InputMouse(args.button, args.buttonState, position, Vector2i(0, 0), args.wheel, args.modifier));
    }
}

void WindowWorld::OnActivate()
{
    Window::OnActivate();

    if (!m_world)
    {
        return;
    }

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldActivate();
    }
}

void WindowWorld::OnDeactivate()
{
    Window::OnDeactivate();

    if (!m_world)
    {
        return;
    }

    for (ListenerList::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
    {
        (*iter)->OnWorldDeactivate();
    }

    m_inputManager.Reset();
}

void WindowWorld::OnLoad()
{
    Window::OnLoad();
    System::RegisterWorldContext(this);
    BeginDialogue();
}

void WindowWorld::OnUnload()
{
    Window::OnUnload();
    System::UnregisterWorldContext(this);
}

void WindowWorld::AddListener(IWorldListener* listener)
{
    m_listeners.push_back(listener);
}

void WindowWorld::RemoveListener(IWorldListener* listener)
{
    ListenerList::iterator iter = find(m_listeners.begin(), m_listeners.end(), listener);
    if (iter != m_listeners.end())
    {
        m_listeners.erase(iter);
    }
}

boost::weak_ptr<Actor> WindowWorld::AddActor(const char* actorAlias, const Vector2i& actorPosition, const char* actorName)
{
    return m_world ? m_world->GetActorManager()->AddActor(actorAlias, actorPosition, actorName) : boost::weak_ptr<Actor>();
}

boost::weak_ptr<const Actor> WindowWorld::GetActor(Token actorId) const
{
    return m_world ? m_world->GetActorManager()->GetActor(actorId) : boost::weak_ptr<Actor>();
}

boost::weak_ptr<Actor> WindowWorld::GetActor(Token actorId)
{
    return m_world ? m_world->GetActorManager()->GetActor(actorId) : boost::weak_ptr<Actor>();
}

boost::weak_ptr<const Actor> WindowWorld::GetActor(const char* actorName) const
{
    return m_world ? m_world->GetActorManager()->GetActor(actorName) : boost::weak_ptr<Actor>();
}

boost::weak_ptr<Actor> WindowWorld::GetActor(const char* actorName)
{
    return m_world ? m_world->GetActorManager()->GetActor(actorName) : boost::weak_ptr<Actor>();
}

boost::weak_ptr<const Actor> WindowWorld::PickActor(const Vector2i& position) const
{
    return m_world ? m_world->GetActorManager()->PickActor(position) : boost::weak_ptr<Actor>();
}

boost::weak_ptr<Actor> WindowWorld::PickActor(const Vector2i& position)
{
    return m_world ? m_world->GetActorManager()->PickActor(position) : boost::weak_ptr<Actor>();
}

void WindowWorld::RemoveActor(Token actorId)
{
    if (m_world)
    {
        m_world->GetActorManager()->RemoveActor(actorId);
    }
}

void WindowWorld::ClearActors()
{
    if (m_world)
    {
        m_world->GetActorManager()->ClearActors();
    }
}

bool WindowWorld::IsLayerVisible(ActorLayer layer) const
{
    return m_world && m_world->GetActorManager()->IsLayerVisible(layer);
}

void WindowWorld::ShowLayer(ActorLayer layer)
{
    if (m_world)
    {
        m_world->GetActorManager()->ShowLayer(layer);
    }
}

void WindowWorld::HideLayer(ActorLayer layer)
{
    if (m_world)
    {
        m_world->GetActorManager()->HideLayer(layer);
    }
}

void WindowWorld::ShowAllLayers()
{
    if (m_world)
    {
        m_world->GetActorManager()->ShowAllLayers();
    }
}

void WindowWorld::HideAllLayers()
{
    if (m_world)
    {
        m_world->GetActorManager()->HideAllLayers();
    }
}

const char* WindowWorld::GetBackdropSpriteAlias() const
{
    return m_world ? m_world->GetBackdropSpriteAlias() : NULL;
}

void WindowWorld::SetBackdropSpriteAlias(const char* alias)
{
    if (m_world)
    {
        m_world->SetBackdropSpriteAlias(alias);
    }
}

void WindowWorld::ShowBackdropSprite()
{
    if (m_world)
    {
        m_world->ShowBackdropSprite();
    }
}

void WindowWorld::HideBackdropSprite()
{
    if (m_world)
    {
        m_world->HideBackdropSprite();
    }
}

bool WindowWorld::IsBackdropSpriteVisible() const
{
    return m_world && m_world->IsBackdropSpriteVisible();
}

void WindowWorld::SetScriptAlias(const char* alias)
{
    if (m_world)
    {
        m_world->SetScriptAlias(alias);
    }
}

const char* WindowWorld::GetScriptAlias() const
{
    return m_world ? m_world->GetScriptAlias() : NULL;
}

void WindowWorld::ShowShapes()
{
    if (m_world)
    {
        m_world->GetActorManager()->ShowShapes();
    }
}

void WindowWorld::HideShapes()
{
    if (m_world)
    {
        m_world->GetActorManager()->HideShapes();
    }
}

bool WindowWorld::AreShapesVisible() const
{
    return m_world && m_world->GetActorManager()->AreShapesVisible();
}

void WindowWorld::SetCameraTarget(boost::shared_ptr<ICameraTarget> target)
{
    if (m_world)
    {
        m_world->GetCamera()->SetTarget(target);
    }
}

void WindowWorld::SetCameraTarget(const Vector2i& target)
{
    if (m_world)
    {
        m_world->GetCamera()->SetTarget(target);
    }
}

Vector2i WindowWorld::GetCameraTarget() const
{
    return m_world ? m_world->GetCamera()->GetTarget() : Vector2i();
}

Rect4i WindowWorld::GetCameraBounds() const
{
    return m_world ? m_world->GetCamera()->Frame(GetBounds()) : Rect4i();
}

bool WindowWorld::IsKeyPressed(MetaKey key) const
{
    return m_inputManager.IsKeyPressed(key);
}

bool WindowWorld::IsKeyTriggered(MetaKey key) const
{
    return m_inputManager.IsKeyTriggered(key);
}

void WindowWorld::BeginDialogue(const Rect4i& padding)
{
    EndDialogue();

    const Vector2i size(
        GetSize().x - padding.x0 - padding.x1,
        GetSize().y - padding.y0 - padding.y1
    );

    m_dialogue = System::LoadDialogue("Test.Test1", size.x);
    m_dialogue->SetPosition(Vector2i(padding.x0, padding.y0));
    m_dialogue->SetSize(size);

    AddChild(m_dialogue);
}

void WindowWorld::EndDialogue()
{
    if (m_dialogue)
    {
        RemoveChild(m_dialogue);
        m_dialogue.reset();
    }
}
