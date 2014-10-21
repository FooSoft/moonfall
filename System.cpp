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
#include "System.h"
#include "EventManager.h"
#include "WindowManager.h"
#include "SurfaceManager.h"
#include "Surface.h"
#include "SpriteManifest.h"
#include "FileManifest.h"
#include "AnimationManifest.h"
#include "FontManifest.h"
#include "ScriptManifest.h"
#include "DialogueManifest.h"
#include "WorldManifest.h"
#include "ActorManifest.h"
#include "ActorScript.h"
#include "WorldScript.h"

static Monostate<EventManager>      s_eventManager;
static Monostate<WindowManager>     s_windowManager;
static Monostate<DialogueManifest>  s_dialogueManifest;
static Monostate<SurfaceManager>    s_surfaceManager;
static Monostate<SpriteManifest>    s_spriteManifest;
static Monostate<FileManifest>      s_fileManifest;
static Monostate<AnimationManifest> s_animationManifest;
static Monostate<FontManifest>      s_fontManifest;
static Monostate<WorldManifest>     s_worldManifest;
static Monostate<ScriptManifest>    s_scriptManifest;
static Monostate<ActorManifest>     s_actorManifest;
static boost::weak_ptr<Surface>     s_framebuffer;
static IWorldContext*               s_worldContext = NULL;

int System::SaveFileManifest(const char* filename, const std::vector<std::string>& files)
{
    return FileManifest::SaveManifest(filename, files);
}

bool System::LoadFileManifest(const char* filename)
{
    return s_fileManifest->LoadManifest(filename);
}

void System::ClearFileManifest()
{
    s_fileManifest->ClearManifest();
}

boost::shared_ptr<const Buffer> System::LoadFile(const char* filename)
{
    return s_fileManifest->LoadFile(filename);
}

bool System::SaveFile(const char* filename, const Buffer& buffer)
{
    return s_fileManifest->SaveFile(filename, buffer);
}

void System::FlushFile(const char* filename)
{
    s_fileManifest->FlushFile(filename);
}

void System::FlushFiles()
{
    s_fileManifest->FlushFiles();
}

boost::weak_ptr<Surface> System::CreateFramebufferSurface(const Vector2i& size, ColorMode mode)
{
    s_framebuffer = s_surfaceManager->CreateFramebufferSurface(size, mode);
    return s_framebuffer;
}

boost::shared_ptr<const Surface> System::LoadSurface(const char* filename)
{
    return s_surfaceManager->LoadSurface(filename);
}

boost::shared_ptr<Surface> System::CreateSurface(const Vector2i& size, ColorMode mode)
{
    return s_surfaceManager->CreateSurface(size, mode);
}

void System::FlushSurfaces()
{
    s_surfaceManager->FlushSurfaces();
}

bool System::LoadSpriteManifest(const char* filename)
{
    return s_spriteManifest->LoadManifest(filename);
}

void System::ClearSpriteManifest()
{
    s_spriteManifest->ClearManifest();
}

boost::shared_ptr<Sprite> System::LoadSprite(const char* spriteAlias)
{
    return s_spriteManifest->LoadSprite(spriteAlias);
}

void System::SummarizeSprites(std::vector<SpriteSummary>* summary)
{
    s_spriteManifest->SummarizeSprites(summary);
}

bool System::LoadAnimationManifest(const char* filename)
{
    return s_animationManifest->LoadManifest(filename);
}

bool System::LoadScriptManifest(const char* filename)
{
    return s_scriptManifest->LoadManifest(filename);
}

void System::ClearScriptManifest()
{
    s_scriptManifest->ClearManifest();
}

boost::shared_ptr<ActorScript> System::LoadActorScript(const char* scriptAlias)
{
    return s_scriptManifest->LoadScript<ActorScript>(scriptAlias);
}

boost::shared_ptr<WorldScript> System::LoadWorldScript(const char* scriptAlias)
{
    return s_scriptManifest->LoadScript<WorldScript>(scriptAlias);
}

boost::shared_ptr<const Buffer> System::LoadScriptCode(const char* scriptAlias)
{
    return s_scriptManifest->LoadScriptCode(scriptAlias);
}

void System::SummarizeScripts(std::vector<ScriptSummary>* summary)
{
    s_scriptManifest->SummarizeScripts(summary);
}

bool System::LoadDialogueManifest(const char* filename)
{
    return s_dialogueManifest->LoadManifest(filename);
}

void System::ClearDialogueManifest()
{
    s_dialogueManifest->ClearManifest();
}

boost::shared_ptr<WindowDialogue> System::LoadDialogue(const char* dialogueAlias, int maxWidth)
{
    return s_dialogueManifest->LoadDialogue(dialogueAlias, maxWidth);
}

bool System::LoadFontManifest(const char* filename)
{
    return s_fontManifest->LoadManifest(filename);
}

void System::ClearFontManifest()
{
    s_fontManifest->ClearManifest();
}

boost::shared_ptr<Font> System::LoadFont(const char* fontAlias)
{
    return s_fontManifest->LoadFont(fontAlias);
}

void System::ClearAnimationManifest()
{
    s_animationManifest->ClearManifest();
}

boost::shared_ptr<Animation> System::LoadAnimation(const char* animationAlias)
{
    return s_animationManifest->LoadAnimation(animationAlias);
}

bool System::LoadActorManifest(const char* filename)
{
    return s_actorManifest->LoadManifest(filename);
}

void System::ClearActorManifest()
{
    s_actorManifest->ClearManifest();
}

boost::shared_ptr<Actor> System::CreateActor(const char* actorAlias, const char* actorName, unsigned actorAllowedProperties)
{
    return s_actorManifest->CreateActor(actorAlias, actorName, actorAllowedProperties);
}

void System::SummarizeActors(std::vector<ActorSummary>* summary)
{
    s_actorManifest->SummarizeActors(summary);
}

bool System::SummarizeActor(const char* actorAlias, ActorSummary* summary)
{
    return s_actorManifest->SummarizeActor(actorAlias, summary);
}

bool System::LoadWorldManifest(const char* filename)
{
    return s_worldManifest->LoadManifest(filename);
}

void System::ClearWorldManifest()
{
    s_worldManifest->ClearManifest();
}

boost::shared_ptr<World> System::LoadWorld(const char* worldAlias, const ParameterMap& parameters)
{
    return s_worldManifest->LoadWorld(worldAlias, parameters);
}

bool System::SaveWorld(const char* worldAlias, const boost::shared_ptr<World>& world)
{
    return s_worldManifest->SaveWorld(worldAlias, world);
}

void System::SummarizeWorlds(std::vector<WorldSummary>* summary)
{
    return s_worldManifest->SummarizeWorlds(summary);
}

void System::RegisterWorldContext(IWorldContext* context)
{
    s_worldContext = context;
}

void System::UnregisterWorldContext(IWorldContext* context)
{
    if (s_worldContext == context)
    {
        s_worldContext = NULL;
    }
}

IWorldContext* System::QueryWorldContext()
{
    return s_worldContext;
}

void System::AddEventHandler(IEventHandler* handler)
{
    s_eventManager->AddHandler(handler);
}

void System::RemoveEventHandler(IEventHandler* handler)
{
    s_eventManager->RemoveHandler(handler);
}

void System::EnqueueWindow(const boost::shared_ptr<Window>& window)
{
    s_windowManager->EnqueueWindow(window);
}

void System::RenderWindow()
{
    const boost::shared_ptr<Surface> framebuffer = s_framebuffer.lock();
    if (framebuffer)
    {
        s_windowManager->Render(framebuffer);
    }
}

void System::UpdateWindow(float elapsed)
{
    const boost::shared_ptr<Surface> framebuffer = s_framebuffer.lock();
    const Vector2i resolution = framebuffer ? framebuffer->GetSize() : Vector2i(0, 0);
    s_eventManager->Update(elapsed);
    s_windowManager->Update(elapsed, resolution);
}

void System::Reset()
{
    // the order of destruction here is very important to make sure
    // that dependant objects are destroyed before their dependencies
    s_dialogueManifest.Reset();
    s_worldManifest.Reset();
    s_actorManifest.Reset();
    s_scriptManifest.Reset();
    s_windowManager.Reset();
    UnregisterWorldContext(s_worldContext);
    s_eventManager.Reset();
    s_animationManifest.Reset();
    s_spriteManifest.Reset();
    s_fontManifest.Reset();
    s_framebuffer.reset();
    s_surfaceManager.Reset();
    s_fileManifest.Reset();
}
