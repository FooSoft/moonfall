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
#include "Surface.h"

Surface::Surface(SDL_Surface* surface) :
        m_surface(surface)
{
    ASSERT(surface != NULL);
    m_activeState.viewport = Rect4i(0, 0, GetSize().x, GetSize().y);
}

Surface::~Surface()
{
    SDL_FreeSurface(m_surface);
}

void Surface::Blit(const boost::shared_ptr<Surface>& target, const Vector2i& sourcePosition, const Vector2i& sourceSize) const
{
    if (!target)
    {
        TRACE_WARNING("Cannot blit to NULL surface");
        return;
    }

    SDL_Rect sourceRect;
    sourceRect.x = sourcePosition.x;
    sourceRect.y = sourcePosition.y;
    sourceRect.w = sourceSize.x;
    sourceRect.h = sourceSize.y;

    SDL_Rect targetRect;
    targetRect.x = target->m_activeState.GetAbsoluteTranslation().x;
    targetRect.y = target->m_activeState.GetAbsoluteTranslation().y;
    targetRect.w = 0; // unused
    targetRect.h = 0; // unused

    SDL_Rect clipRect;
    clipRect.x = target->m_activeState.viewport.x0;
    clipRect.y = target->m_activeState.viewport.y0;
    clipRect.w = target->m_activeState.viewport.x1 - target->m_activeState.viewport.x0;
    clipRect.h = target->m_activeState.viewport.y1 - target->m_activeState.viewport.y0;

    SDL_SetClipRect(target->m_surface, &clipRect);
    SDL_BlitSurface(m_surface, &sourceRect, target->m_surface, &targetRect);
}

void Surface::SetViewportRelative(const Rect4i& viewport)
{
    State stateBase;
    if (!m_stateStack.empty())
    {
        stateBase = m_stateStack.top();
    }

    m_activeState.viewport = Rect4i(
                                 stateBase.viewport.x0 + viewport.x0,
                                 stateBase.viewport.y0 + viewport.y0,
                                 stateBase.viewport.x0 + viewport.x1,
                                 stateBase.viewport.y0 + viewport.y1
                             );
}

void Surface::SetViewport(const Rect4i& viewport)
{
    m_activeState.viewport = viewport;
}

Rect4i Surface::GetViewport() const
{
    return m_activeState.viewport;
}

void Surface::SetTranslationRelative(const Vector2i& translation)
{
    m_activeState.translation += translation;
}

void Surface::SetTranslation(const Vector2i& translation)
{
    m_activeState.translation = translation;
}

void Surface::PushState()
{
    m_stateStack.push(m_activeState);
}

void Surface::PopState()
{
    ASSERT(!m_stateStack.empty());
    m_activeState = m_stateStack.top();
    m_stateStack.pop();
}

void Surface::Clear(const Color4b& color, const Vector2i& size)
{
    SDL_Rect targetRect;
    targetRect.x = m_activeState.GetAbsoluteTranslation().x;
    targetRect.y = m_activeState.GetAbsoluteTranslation().y;
    targetRect.w = MIN(size.x, m_activeState.GetViewportSize().x);
    targetRect.h = MIN(size.y, m_activeState.GetViewportSize().y);

    SDL_FillRect(
        m_surface,
        &targetRect,
        SDL_MapRGBA(m_surface->format, color.red, color.green, color.blue, color.alpha)
    );
}

void Surface::Clear(const Color4b& color)
{
    Clear(color, GetSize());
}

void Surface::Flip()
{
    SDL_Flip(m_surface);
}

void Surface::SetAlpha(int flags, byte alpha)
{
    SDL_SetAlpha(m_surface, flags, alpha);
}

void Surface::SetColorKey(const Color4b& key, bool useRle)
{
    SDL_SetColorKey(m_surface, useRle ? SDL_SRCCOLORKEY | SDL_RLEACCEL : SDL_SRCCOLORKEY, SDL_MapRGB(m_surface->format, key.red, key.green, key.blue));
}

void Surface::ClearColorKey()
{
    SDL_SetColorKey(m_surface, 0, 0);
}

Vector2i Surface::GetSize () const
{
    return Vector2i(m_surface->w, m_surface->h);
}

const void* Surface::GetData() const
{
    return m_surface->pixels;
}

void* Surface::GetData()
{
    return m_surface->pixels;
}

void Surface::Lock()
{
    if (SDL_MUSTLOCK(m_surface))
    {
        SDL_LockSurface(m_surface);
    }
}

void Surface::Unlock()
{
    if (SDL_MUSTLOCK(m_surface))
    {
        SDL_UnlockSurface(m_surface);
    }
}
