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
#include "DialogueManifest.h"
#include "WindowDialogue.h"

bool DialogueManifest::LoadManifest(const char* filename)
{
    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load dialogue manifest file %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse dialogue manifest file %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* baseElement = rootElement->FirstChildElement(); baseElement != NULL; baseElement = baseElement->NextSiblingElement())
    {
        if (strcmp(baseElement->Value(), "Dialogue") != 0)
        {
            continue;
        }

        const char* asset = baseElement->Attribute("asset");
        const char* alias = baseElement->Attribute("alias");

        if (asset == NULL || alias == NULL)
        {
            TRACE_WARNING("Dialogue definition is missing required fields");
            continue;
        }

        if (m_manifest.find(alias) != m_manifest.end())
        {
            TRACE_WARNING(boost::format("Duplicate dialogue %s") % alias);
            continue;
        }

        m_manifest.insert(std::make_pair(alias, asset));
    }

    return true;
}

void DialogueManifest::ClearManifest()
{
    m_manifest.clear();
}

boost::shared_ptr<WindowDialogue> DialogueManifest::LoadDialogue(const char* dialogueAlias, int maxWidth) const
{
    const DialogueMap::const_iterator iter = m_manifest.find(dialogueAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load dialogue %s") % dialogueAlias);
        return boost::shared_ptr<WindowDialogue>();
    }

    const char* filename = iter->second.c_str();

    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load dialogue file %s") % filename);
        return boost::shared_ptr<WindowDialogue>();
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse dialogue file %s") % filename);
        return boost::shared_ptr<WindowDialogue>();
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Dialogue") != 0)
    {
        return boost::shared_ptr<WindowDialogue>();
    }

    const boost::shared_ptr<WindowDialogue> window(new WindowDialogue());

    for (const TiXmlElement* baseElement = rootElement->FirstChildElement(); baseElement != NULL; baseElement = baseElement->NextSiblingElement())
    {
        if (strcmp(baseElement->Value(), "Screen") != 0)
        {
            continue;
        }

        window->PushScreen(baseElement->Attribute("name"));

        for (const TiXmlElement* screenElement = baseElement->FirstChildElement(); screenElement != NULL; screenElement = screenElement->NextSiblingElement())
        {
            if (strcmp(screenElement->Value(), "Panel") != 0)
            {
                continue;
            }

            window->PushScreenPanel(screenElement->Attribute("text"), screenElement->Attribute("title"), screenElement->Attribute("sprite"), screenElement->Attribute("jump"));

            for (const TiXmlElement* panelElement = screenElement->FirstChildElement(); panelElement != NULL; panelElement = panelElement->NextSiblingElement())
            {
                if (strcmp(panelElement->Value(), "Option") == 0)
                {
                    window->PushScreenPanelOption(panelElement->Attribute("text"), panelElement->Attribute("value"), panelElement->Attribute("jump"));
                }
            }
        }
    }

    return window;
}
