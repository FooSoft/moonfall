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
#include "Animation.h"
#include "Sprite.h"

Animation::Animation(const std::vector<Frame>& frames) :
        m_frames(frames),
        m_elapsed(0),
        m_index(0),
        m_loop(false),
        m_play(false)
{
    ASSERT(frames.size() > 0);
}

void Animation::Update(float elapsed)
{
    if (!m_play)
    {
        return;
    }

    m_elapsed += elapsed;

    while (m_elapsed > m_frames[m_index].delay)
    {
        m_elapsed -= m_frames[m_index++].delay;

        if (m_index == static_cast<int>(m_frames.size()))
        {
            if (m_loop)
            {
                m_index = 0;
            }
            else
            {
                m_play = false;
                --m_index;
            }
        }
    }
}

void Animation::Render(const boost::shared_ptr<Surface>& target)
{
    m_frames[m_index].sprite->Render(target);
}

Vector2i Animation::GetSize() const
{
    return m_frames[m_index].sprite->GetSize();
}

const ActorShape* Animation::GetShapes() const
{
    return m_frames[m_index].sprite->GetShapes();
}

int Animation::GetFrameCount() const
{
    return static_cast<int>(m_frames.size());
}

int Animation::GetFrame() const
{
    return m_index;
}

void Animation::Play(bool loop)
{
    m_loop = loop;
    m_play = true;
}

void Animation::Stop()
{
    Rewind();
    Pause();
}

void Animation::Rewind()
{
    m_index = 0;
}

void Animation::Pause()
{
    m_play = false;
}

bool Animation::IsPlaying() const
{
    return m_play;
}
