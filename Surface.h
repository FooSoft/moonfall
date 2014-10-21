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

class Surface : private boost::noncopyable
{
public:
    Surface(SDL_Surface* surface);
    ~Surface();

    void                Blit(const boost::shared_ptr<Surface>& target, const Vector2i& sourcePosition, const Vector2i& sourceSize) const;
    void                SetViewportRelative(const Rect4i& viewport);
    void                SetViewport(const Rect4i& viewport);
    Rect4i              GetViewport() const;
    void                SetTranslationRelative(const Vector2i& translation);
    void                SetTranslation(const Vector2i& translation);
    void                PushState();
    void                PopState();

    Vector2i            GetSize() const;
    const void*         GetData() const;
    void*               GetData();

    void                SetAlpha(int flags, byte alpha);
    void                SetColorKey(const Color4b& key, bool useRle = false);
    void                ClearColorKey();

    void                Clear(const Color4b& color, const Vector2i& size);
    void                Clear(const Color4b& color);
    void                Lock();
    void                Unlock();
    void                Flip();

private:
    struct State
    {
        State(const Rect4i& viewport = Rect4i(0), const Vector2i& translation = Vector2i(0)) :
                viewport(viewport), translation(translation) { }

        Vector2i        GetViewportSize() const { return Vector2i(viewport.GetWidth(), viewport.GetHeight()); }
        Vector2i        GetAbsoluteTranslation() const { return Vector2i(viewport.x0, viewport.y0) + translation; }

        Rect4i          viewport;
        Vector2i        translation;
    };

    SDL_Surface*        m_surface;
    std::stack<State>   m_stateStack;
    State               m_activeState;
};
