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

#ifndef PCH_H
#define PCH_H

// Environment
#include "Environment.h"

// CRT
#include <stdlib.h>

// STL
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>

// wxWidgets
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/scrolwin.h>
#include <wx/imaglist.h>
#include <wx/choicebk.h>
#include <wx/grid.h>
#include <wx/statline.h>
#include <wx/splitter.h>
#include <wx/choicdlg.h>
#include <wx/listctrl.h>
#include <wx/filename.h>
#include <wx/dcbuffer.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>

// SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Boost
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/format.hpp>
#include <boost/utility.hpp>
#include <boost/static_assert.hpp>

// Lua
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "tinyxml.h"

// Utility
#include "Define.h"
#include "Assert.h"
#include "Monostate.h"
#include "Buffer.h"
#include "Trace.h"
#include "HeapTracker.h"
#include "Token.h"
#include "Vector.h"
#include "Rect.h"
#include "Color.h"
#include "Grid.h"
#include "RefCount.h"
#include "Serialization.h"
#include "Compression.h"

// Engine
#include "EngineCommon.h"
#include "GameCommon.h"
#include "System.h"

#endif
