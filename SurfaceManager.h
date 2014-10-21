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

class SurfaceManager
{
public:
    boost::weak_ptr<Surface>            CreateFramebufferSurface(const Vector2i& size, ColorMode mode);
    boost::weak_ptr<Surface>            GetFramebufferSurface();

    boost::shared_ptr<Surface>          CreateSurface(const Vector2i& size, ColorMode mode);
    boost::shared_ptr<const Surface>    LoadSurface(const char* filename);

    void                                FlushSurfaces();

private:
    struct ColorFormat
    {
        int         depth;
        Color4d     mask;
        unsigned    flags;
    };

    static const ColorFormat&           GetColorFormat(ColorMode mode);

    boost::shared_ptr<Surface>          m_framebuffer;
    RefCount<const Surface>             m_manager;
};
