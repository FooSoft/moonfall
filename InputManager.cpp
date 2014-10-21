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
#include "InputManager.h"

InputManager::InputManager() :
        m_keyTriggerFlags(0),
        m_keyFlags(0)
{
}

bool InputManager::IsKeyPressed(MetaKey key) const
{
    return IS_TRUE(m_keyFlags & BIT(key));
}

bool InputManager::IsKeyTriggered(MetaKey key) const
{
    return IS_TRUE(m_keyTriggerFlags & BIT(key));
}

void InputManager::DoKeyDown(const InputKey& args)
{
    MetaKey key = META_KEY_COUNT;
    if (TranslateKey(args, &key) && IS_FALSE(m_keyFlags & BIT(key)))
    {
        m_keyFlags |= BIT(key);
        m_keyTriggerFlags |= BIT(key);
    }
}

void InputManager::DoKeyUp(const InputKey& args)
{
    MetaKey key = META_KEY_COUNT;
    if (TranslateKey(args, &key) && IS_TRUE(m_keyFlags & BIT(key)))
    {
        m_keyFlags &= ~BIT(key);
        m_keyTriggerFlags |= BIT(key);
    }
}

void InputManager::Update(float elapsed)
{
    m_keyTriggerFlags = 0;
}

void InputManager::Reset()
{
    m_keyTriggerFlags = 0;
    m_keyFlags = 0;
}

void InputManager::ReleaseKeys()
{
    m_keyTriggerFlags = static_cast<unsigned>(-1);
    m_keyFlags = 0;
}

bool InputManager::TranslateKey(const InputKey& args, MetaKey* key)
{
    switch (args.key)
    {
    case SDLK_UP:
        *key = META_KEY_UP;
        return true;
    case SDLK_DOWN:
        *key = META_KEY_DOWN;
        return true;
    case SDLK_LEFT:
        *key = META_KEY_LEFT;
        return true;
    case SDLK_RIGHT:
        *key = META_KEY_RIGHT;
        return true;
    case SDLK_SPACE:
        *key = META_KEY_USE;
        return true;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
        *key = META_KEY_RUN;
        return true;
    default:
        return false;
    }
}
