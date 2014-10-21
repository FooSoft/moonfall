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

class AnimationManifest
{
public:
    bool                            LoadManifest(const char* filename);
    void                            ClearManifest();

    boost::shared_ptr<Animation>    LoadAnimation(const char* animationAlias) const;

private:
    struct AnimationFrame
    {
        AnimationFrame(const std::string& alias, float delay) :
                alias(alias), delay(delay) { }

        std::string                 alias;
        float                       delay;
    };

    struct AnimationEntry
    {
        std::vector<AnimationFrame> frames;
    };

    typedef                         std::map<std::string, AnimationEntry> AnimationMap;
    AnimationMap                    m_manifest;
};
