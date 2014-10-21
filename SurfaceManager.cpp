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
#include "SurfaceManager.h"
#include "Surface.h"

boost::shared_ptr<Surface> SurfaceManager::CreateSurface(const Vector2i& size, ColorMode mode)
{
    const ColorFormat& format = GetColorFormat(mode);

    SDL_Surface* surface = SDL_CreateRGBSurface(
                               format.flags,
                               size.x,
                               size.y,
                               format.depth,
                               format.mask.red,
                               format.mask.green,
                               format.mask.blue,
                               format.mask.alpha
                           );

    if (surface == NULL)
    {
        TRACE_ERROR(boost::format("Cannot create surface for color mode %d") % mode);
        return boost::shared_ptr<Surface>();
    }

    return boost::shared_ptr<Surface>(new Surface(surface));
}

boost::shared_ptr<const Surface> SurfaceManager::LoadSurface(const char* filename)
{
    if (boost::shared_ptr<const Surface> surface = m_manager.FindClient(filename))
    {
        return surface;
    }

    boost::shared_ptr<const Buffer> buffer = System::LoadFile(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Cannot load surface from file %s") % filename);
        return boost::shared_ptr<const Surface>();
    }

    SDL_RWops* memory = SDL_RWFromConstMem(buffer->Get(), buffer->GetSize());
    SDL_RWwrite(memory, buffer->Get(), 1, buffer->GetSize());
    boost::shared_ptr<const Surface> surface(new Surface(IMG_LoadPNG_RW(memory)));
    SDL_FreeRW(memory);

    m_manager.AddClient(filename, surface);
    return surface;
}

boost::weak_ptr<Surface> SurfaceManager::CreateFramebufferSurface(const Vector2i& size, ColorMode mode)
{
    const ColorFormat& format = GetColorFormat(mode);

    SDL_Surface* surface = SDL_SetVideoMode(size.x, size.y, format.depth, format.flags);
    if (surface == NULL)
    {
        TRACE_ERROR(boost::format("Cannot create framebuffer surface with color mode %d") % mode);
        m_framebuffer.reset();
    }
    else
    {
        m_framebuffer = boost::shared_ptr<Surface>(new Surface(surface));
    }

    return m_framebuffer;
}

boost::weak_ptr<Surface> SurfaceManager::GetFramebufferSurface()
{
    return m_framebuffer;
}

const SurfaceManager::ColorFormat& SurfaceManager::GetColorFormat(ColorMode mode)
{
    static const ColorFormat s_formats[] =
    {
        // COLOR_MODE_RGB_888
        {
            24,
            Color4d(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000),
            SDL_HWSURFACE
        },

        // COLOR_MODE_RGBA_8888
        {
            32,
            Color4d(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff),
            SDL_HWSURFACE | SDL_DOUBLEBUF
        }
    };

    ASSERT(mode < static_cast<int>(ARRAY_SIZE(s_formats)));
    return s_formats[mode];
}

void SurfaceManager::FlushSurfaces()
{
    m_manager.ClearClients();
    TRACE_INFO("Flushed cached surfaces");
}
