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

class   ActorScript;
class   WorldScript;
class   Surface;
class   Sprite;
class   Animation;
class   Actor;
class   Window;
class   WindowDialogue;
class   Font;
class   IWorldContext;
class   IEventHandler;
struct  World;
struct  ActorSummary;
struct  SpriteSummary;
struct  WorldSummary;
struct  ScriptSummary;
struct  ScriptParameter;
struct  InputKey;
struct  InputMouse;

namespace System
{
    // File
    int                                 SaveFileManifest(const char* filename, const std::vector<std::string>& files);
    bool                                LoadFileManifest(const char* filename);
    void                                ClearFileManifest();
    boost::shared_ptr<const Buffer>     LoadFile(const char* filename);
    bool                                SaveFile(const char* filename, const Buffer& buffer);
    void                                FlushFile(const char* filename);
    void                                FlushFiles();

    // Surface
    boost::weak_ptr<Surface>            CreateFramebufferSurface(const Vector2i& size, ColorMode mode);
    boost::shared_ptr<Surface>          CreateSurface(const Vector2i& size, ColorMode mode);
    boost::shared_ptr<const Surface>    LoadSurface(const char* filename);
    void                                FlushSurfaces();

    // Sprite
    bool                                LoadSpriteManifest(const char* filename);
    void                                ClearSpriteManifest();
    boost::shared_ptr<Sprite>           LoadSprite(const char* spriteAlias);
    void                                SummarizeSprites(std::vector<SpriteSummary>* summary);

    // Animation
    bool                                LoadAnimationManifest(const char* filename);
    void                                ClearAnimationManifest();
    boost::shared_ptr<Animation>        LoadAnimation(const char* animationAlias);

    // Font
    bool                                LoadFontManifest(const char* filename);
    void                                ClearFontManifest();
    boost::shared_ptr<Font>             LoadFont(const char* fontAlias);

    // Script
    bool                                LoadScriptManifest(const char* filename);
    void                                ClearScriptManifest();
    boost::shared_ptr<ActorScript>      LoadActorScript(const char* scriptAlias);
    boost::shared_ptr<WorldScript>      LoadWorldScript(const char* scriptAlias);
    boost::shared_ptr<const Buffer>     LoadScriptCode(const char* scriptAlias);
    void                                SummarizeScripts(std::vector<ScriptSummary>* summary);

    // Dialogue
    bool                                LoadDialogueManifest(const char* filename);
    void                                ClearDialogueManifest();
    boost::shared_ptr<WindowDialogue>   LoadDialogue(const char* dialogueAlias, int maxWidth);

    // Actor
    bool                                LoadActorManifest(const char* filename);
    void                                ClearActorManifest();
    void                                SummarizeActors(std::vector<ActorSummary>* summary);
    bool                                SummarizeActor(const char* actorAlias, ActorSummary* summary);
    boost::shared_ptr<Actor>            CreateActor(const char* actorAlias, const char* actorName, unsigned actorAllowedProperties = static_cast<unsigned>(-1));

    // World
    bool                                LoadWorldManifest(const char* filename);
    void                                ClearWorldManifest();
    boost::shared_ptr<World>            LoadWorld(const char* worldAlias, const ParameterMap& parameters = ParameterMap());
    bool                                SaveWorld(const char* worldAlias, const boost::shared_ptr<World>& world);
    void                                SummarizeWorlds(std::vector<WorldSummary>* summary);

    // Editor
    void                                RegisterWorldContext(IWorldContext* context);
    void                                UnregisterWorldContext(IWorldContext* context);
    IWorldContext*                      QueryWorldContext();

    // Event
    void                                AddEventHandler(IEventHandler* handler);
    void                                RemoveEventHandler(IEventHandler* handler);

    // Window
    void                                EnqueueWindow(const boost::shared_ptr<Window>& window);
    void                                RenderWindow();
    void                                UpdateWindow(float elapsed);

    // Misc
    void Reset();
}
