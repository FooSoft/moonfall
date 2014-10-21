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

#ifdef OS_WINDOWS
    #define ADJUST_FILENAME(filename) \
        { \
            static char filenameWin[PATH_MAX] = {0}; \
            for (int i = 0; filename[i] != 0; ++i) \
            { \
                filenameWin[i] = filename[i] == '/' ? '\\' : filename[i]; \
            } \
            filename = filenameWin; \
        }
#else
    #define ADJUST_FILENAME(filename)
#endif

#ifdef OS_WINDOWS
    const char PATH_SEPARATOR = '\\';
#else
    const char PATH_SEPARATOR = '/';
#endif

enum ColorMode
{
    COLOR_MODE_RGB_888,
    COLOR_MODE_RGBA_8888
};

struct InputKey
{
    InputKey(SDLKey key, int modifier, int scancode, int unicode) :
            key(key), modifier(modifier), scancode(scancode), unicode(unicode) { }

    SDLKey  key;
    int     modifier;
    int     scancode;
    int     unicode;
};

struct InputMouse
{
    InputMouse(int button, int buttonState, const Vector2i& position, const Vector2i& delta, int wheel, int modifier) :
            button(button), buttonState(buttonState), position(position), delta(delta), wheel(wheel), modifier(modifier) { }

    int         button;
    int         buttonState;
    Vector2i    position;
    Vector2i    delta;
    int         wheel;
    int         modifier;
};
