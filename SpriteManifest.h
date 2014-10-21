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

class SpriteManifest
{
public:
    bool                        LoadManifest(const char* filename);
    bool                        SaveManifest(const char* filename) const;
    void                        ClearManifest();
    bool                        AppendManifest(const char* alias, const char* asset, const Vector2i& position, const Vector2i& size, const ActorShape shapes[]);

    boost::shared_ptr<Sprite>   LoadSprite(const char* spriteAlias) const;
    void                        SummarizeSprites(std::vector<SpriteSummary>* summary) const;

private:
    struct SpriteEntry
    {
        SpriteEntry(const std::string& asset, const Vector2i& position, const Vector2i& size, const ActorShape shapes[]) :
                asset(asset), position(position), size(size)
        {
            memcpy(this->shapes, shapes, sizeof(this->shapes));
        }

        ActorShape              shapes[ACTOR_SHAPE_TYPE_COUNT];
        std::string             asset;
        Vector2i                position;
        Vector2i                size;
    };

    typedef                     std::map<std::string, SpriteEntry> SpriteMap;
    SpriteMap                   m_manifest;
};
