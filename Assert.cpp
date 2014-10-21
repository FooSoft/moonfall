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
#include "Assert.h"

void AssertFail(const char* expression, const char* filename, int line)
{
    const char* body = strrchr(filename, PATH_SEPARATOR);
    if (body != NULL)
    {
        filename = body + 1;
    }

    TRACE_ERROR(boost::format("Assertion \"%s\" failed in %s:%d") % expression % filename % line);
    abort();
}
