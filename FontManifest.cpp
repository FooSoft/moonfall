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
#include "FontManifest.h"
#include "Font.h"

bool FontManifest::LoadManifest(const char* filename)
{
    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load font manifest file %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse font manifest file %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* baseElement = rootElement->FirstChildElement(); baseElement != NULL; baseElement = baseElement->NextSiblingElement())
    {
        if (strcmp(baseElement->Value(), "Font") != 0)
        {
            continue;
        }

        const char* asset = baseElement->Attribute("asset");
        const char* alias = baseElement->Attribute("alias");

        if (asset == NULL || alias == NULL)
        {
            TRACE_WARNING("Font definition is missing required fields");
            continue;
        }

        if (m_manifest.find(alias) != m_manifest.end())
        {
            TRACE_WARNING(boost::format("Duplicate Font %s") % alias);
            continue;
        }

        int red = 0xff;
        baseElement->QueryIntAttribute("red", &red);
        int green = 0xff;
        baseElement->QueryIntAttribute("green", &green);
        int blue = 0xff;
        baseElement->QueryIntAttribute("blue", &blue);

        int size = 16;
        baseElement->QueryIntAttribute("size", &size);

        m_manifest.insert(std::make_pair(alias, FontEntry(asset, Color4b(red, green, blue, 0xff), size)));
    }

    return true;
}

void FontManifest::ClearManifest()
{
    m_manifest.clear();
}

boost::shared_ptr<Font> FontManifest::LoadFont(const char* fontAlias) const
{
    const FontMap::const_iterator iter = m_manifest.find(fontAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load font %s") % fontAlias);
        return boost::shared_ptr<Font>();
    }

    const char* filename = iter->second.filename.c_str();
    const boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load font from file %s") % filename);
        return boost::shared_ptr<Font>();
    }

    SDL_RWops* memory = SDL_RWFromConstMem(buffer->Get(), buffer->GetSize());
    SDL_RWwrite(memory, buffer->Get(), 1, buffer->GetSize());
    boost::shared_ptr<Font> font(new Font(TTF_OpenFontRW(memory, 0, iter->second.size), iter->second.color));

    return font;
}
