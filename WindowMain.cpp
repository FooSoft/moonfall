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
#include "WindowMain.h"
#include "WindowWorld.h"

void WindowMain::OnLoad()
{
    const bool success =
        System::LoadFileManifest("Assets.pak") &&
        System::LoadSpriteManifest("Assets/SpriteManifest.xml") &&
        System::LoadAnimationManifest("Assets/AnimationManifest.xml") &&
        System::LoadScriptManifest("Assets/ScriptManifest.xml") &&
        System::LoadFontManifest("Assets/FontManifest.xml") &&
        System::LoadActorManifest("Assets/ActorManifest.xml") &&
        System::LoadWorldManifest("Assets/WorldManifest.xml") &&
        System::LoadDialogueManifest("Assets/DialogueManifest.xml");

    if (success)
    {
        System::EnqueueWindow(boost::shared_ptr<Window>(new WindowWorld("Test.Test1")));
    }
}
