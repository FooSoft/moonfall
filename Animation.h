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
#include "Actor.h"

class Animation : private boost::noncopyable
{
public:
    struct Frame
    {
        Frame(const boost::shared_ptr<Sprite>& sprite, float delay) :
                sprite(sprite), delay(delay) { }

        boost::shared_ptr<Sprite>   sprite;
        float                       delay;
    };

    Animation(const std::vector<Frame>& frames);

    void                            Update(float elapsed);
    void                            Render(const boost::shared_ptr<Surface>& target);

    Vector2i                        GetSize() const;
    const ActorShape*               GetShapes() const;
    int                             GetFrameCount() const;
    int                             GetFrame() const;

    void                            Play(bool loop);
    void                            Stop();
    void                            Rewind();
    void                            Pause();
    bool                            IsPlaying() const;

private:
    std::vector<Frame>              m_frames;
    float                           m_elapsed;
    int                             m_index;
    bool                            m_loop;
    bool                            m_play;
};
