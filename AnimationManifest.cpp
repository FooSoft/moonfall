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
#include "AnimationManifest.h"
#include "Animation.h"

bool AnimationManifest::LoadManifest(const char* filename)
{
    boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load animation manifest %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse animation manifest %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* animationElement = rootElement->FirstChildElement(); animationElement != NULL; animationElement = animationElement->NextSiblingElement())
    {
        if (strcmp(animationElement->Value(), "Animation") != 0)
        {
            continue;
        }

        const char* alias = animationElement->Attribute("alias");
        if (alias == NULL)
        {
            TRACE_WARNING("Animation definition is missing required fields");
            continue;
        }

        if (m_manifest.find(alias) != m_manifest.end())
        {
            TRACE_WARNING(boost::format("Duplicate animation %s") % alias);
            continue;
        }

        float defaultDelay = 1;
        animationElement->QueryFloatAttribute("delay", &defaultDelay);

        AnimationEntry entry;
        for (const TiXmlElement* spriteElement = animationElement->FirstChildElement(); spriteElement != NULL; spriteElement = spriteElement->NextSiblingElement())
        {
            if (strcmp(spriteElement->Value(), "Frame") != 0)
            {
                continue;
            }

            const char* sprite = spriteElement->Attribute("sprite");
            if (sprite == NULL)
            {
                TRACE_WARNING("Animation frame definition is missing required fields");
                continue;
            }

            float delay = defaultDelay;
            spriteElement->QueryFloatAttribute("delay", &delay);

            entry.frames.push_back(AnimationFrame(sprite, delay));
        }

        m_manifest.insert(std::make_pair(alias, entry));
    }

    return true;
}

void AnimationManifest::ClearManifest()
{
    m_manifest.clear();
}

boost::shared_ptr<Animation> AnimationManifest::LoadAnimation(const char* animationAlias) const
{
    AnimationMap::const_iterator iter = m_manifest.find(animationAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load animation %s") % animationAlias);
        return boost::shared_ptr<Animation>();
    }

    std::vector<Animation::Frame> frames;
    for (size_t i = 0; i < iter->second.frames.size(); ++i)
    {
        boost::shared_ptr<Sprite> sprite = System::LoadSprite(iter->second.frames[i].alias.c_str());
        if (!sprite)
        {
            return boost::shared_ptr<Animation>();
        }

        frames.push_back(Animation::Frame(sprite, iter->second.frames[i].delay));
    }

    return boost::shared_ptr<Animation>(new Animation(frames));
}
