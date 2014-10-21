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
#include "SpriteManifest.h"
#include "Sprite.h"
#include "Surface.h"
#include "Actor.h"

bool SpriteManifest::LoadManifest(const char* filename)
{
    boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load sprite manifest %s") % filename);
        return false;
    }

    TiXmlDocument document;
    if (!document.Parse(static_cast<const char*>(buffer->Get())))
    {
        TRACE_ERROR(boost::format("Cannot parse sprite manifest %s") % filename);
        return false;
    }

    const TiXmlElement* rootElement = document.RootElement();
    if (rootElement == NULL || strcmp(rootElement->Value(), "Manifest") != 0)
    {
        return false;
    }

    for (const TiXmlElement* spriteElement = rootElement->FirstChildElement(); spriteElement != NULL; spriteElement = spriteElement->NextSiblingElement())
    {
        if (strcmp(spriteElement->Value(), "Sprite") != 0)
        {
            continue;
        }

        const char* asset = spriteElement->Attribute("asset");
        const char* alias = spriteElement->Attribute("alias");

        if (asset == NULL || alias == NULL)
        {
            TRACE_WARNING("Sprite definition is missing required fields");
            continue;
        }

        Vector2i position;
        spriteElement->Attribute("x", &position.x);
        spriteElement->Attribute("y", &position.y);

        Vector2i size;
        spriteElement->Attribute("width", &size.x);
        spriteElement->Attribute("height", &size.y);

        ActorShape shapes[ACTOR_SHAPE_TYPE_COUNT];
        for (const TiXmlElement* shapeElement = spriteElement->FirstChildElement(); shapeElement != NULL; shapeElement = shapeElement->NextSiblingElement())
        {
            if (strcmp(shapeElement->Value(), "Shape") != 0)
            {
                continue;
            }

            int type = 0;
            if (shapeElement->QueryIntAttribute("type", &type) != TIXML_SUCCESS || type >= ACTOR_SHAPE_TYPE_COUNT)
            {
                TRACE_WARNING(boost::format("Cannot import shape because of missing or invalid type in sprite %s") % alias);
                continue;
            }

            ActorShape& shape = shapes[type];
            shapeElement->QueryIntAttribute("data", &shape.data);
            shapeElement->QueryIntAttribute("width", &shape.size.x);
            shapeElement->QueryIntAttribute("height", &shape.size.y);
            shapeElement->QueryIntAttribute("x", &shape.position.x);
            shapeElement->QueryIntAttribute("y", &shape.position.y);
            shape.enabled = true;
        }

        AppendManifest(alias, asset, position, size, shapes);
    }

    return true;
}

bool SpriteManifest::SaveManifest(const char* filename) const
{
    TiXmlDocument document;

    TiXmlElement* rootElement = new TiXmlElement("Manifest");
    document.LinkEndChild(rootElement);

    for (SpriteMap::const_iterator spriteIter = m_manifest.begin(); spriteIter != m_manifest.end(); ++spriteIter)
    {
        TiXmlElement* spriteElement = new TiXmlElement("Sprite");
        spriteElement->SetAttribute("alias", spriteIter->first.c_str());
        spriteElement->SetAttribute("asset", spriteIter->second.asset.c_str());
        spriteElement->SetAttribute("x", spriteIter->second.position.x);
        spriteElement->SetAttribute("y", spriteIter->second.position.y);
        spriteElement->SetAttribute("width", spriteIter->second.size.x);
        spriteElement->SetAttribute("height", spriteIter->second.size.y);

        for (int i = 0; i < ARRAY_SIZE(spriteIter->second.shapes); ++i)
        {
            const ActorShape& shape = spriteIter->second.shapes[i];
            if (!shape.enabled)
            {
                continue;
            }

            TiXmlElement* shapeElement = new TiXmlElement("Shape");
            shapeElement->SetAttribute("type", i);
            shapeElement->SetAttribute("data", shape.data);
            shapeElement->SetAttribute("width", shape.size.x);
            shapeElement->SetAttribute("height", shape.size.y);
            shapeElement->SetAttribute("x", shape.position.x);
            shapeElement->SetAttribute("y", shape.position.y);
            spriteElement->LinkEndChild(shapeElement);
        }

        rootElement->LinkEndChild(spriteElement);
    }

    System::FlushFile(filename);
    return document.SaveFile(filename);
}

void SpriteManifest::ClearManifest()
{
    m_manifest.clear();
}

bool SpriteManifest::AppendManifest(const char* alias, const char* asset, const Vector2i& position, const Vector2i& size, const ActorShape shapes[])
{
    if (m_manifest.find(alias) != m_manifest.end())
    {
        TRACE_WARNING(boost::format("Duplicate sprite %s") % alias);
        return false;
    }

    m_manifest.insert(std::make_pair(alias, SpriteEntry(asset, position, size, shapes)));
    return true;
}

boost::shared_ptr<Sprite> SpriteManifest::LoadSprite(const char* spriteAlias) const
{
    SpriteMap::const_iterator iter = m_manifest.find(spriteAlias);
    if (iter == m_manifest.end())
    {
        TRACE_ERROR(boost::format("Cannot load sprite %s") % spriteAlias);
        return boost::shared_ptr<Sprite>();
    }

    boost::shared_ptr<const Surface> surface = System::LoadSurface(iter->second.asset.c_str());
    if (!surface)
    {
        TRACE_ERROR(boost::format("Unsupported sprite format %s") % spriteAlias);
        return boost::shared_ptr<Sprite>();
    }

    Vector2i spriteSize = iter->second.size;
    if (spriteSize.x == 0)
    {
        spriteSize.x = surface->GetSize().x;
    }
    if (spriteSize.y == 0)
    {
        spriteSize.y = surface->GetSize().y;
    }

    return boost::shared_ptr<Sprite>(new Sprite(surface, iter->second.position, spriteSize, iter->second.shapes));
}

void SpriteManifest::SummarizeSprites(std::vector<SpriteSummary>* summary) const
{
    for (SpriteMap::const_iterator iter = m_manifest.begin(); iter != m_manifest.end(); ++iter)
    {
        summary->push_back(
            SpriteSummary(
                iter->first,
                iter->second.asset,
                iter->second.position,
                iter->second.size,
                iter->second.shapes
            )
        );
    }
}
