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
#include "Trace.h"

namespace Trace
{

    struct Target
    {
        Target(ITarget* target, Level level) :
                target(target),
                level(level)
        {
        }

        ITarget*    target;
        Level       level;
    };

}

static std::vector<Trace::Target> s_targets;

void Trace::AddTarget(ITarget* target, Level level)
{
    s_targets.push_back(Target(target, level));
}

void Trace::RemoveTarget(ITarget* target)
{
    for (std::vector<Target>::iterator iter = s_targets.begin(); iter != s_targets.end(); ++iter)
    {
        if (iter->target == target)
        {
            s_targets.erase(iter);
            break;
        }
    }
}

void Trace::ClearTargets()
{
    s_targets.clear();
}

static void WriteInternal(const char* text, Trace::Level level, const char* context)
{
    for (std::vector<Trace::Target>::iterator iter = s_targets.begin(); iter != s_targets.end(); ++iter)
    {
        if (level >= iter->level)
        {
            iter->target->Trace(level, text, context);
        }
    }
}

void Trace::Write(const boost::format format, Level level, const char* context)
{
    Write(boost::str(format), level, context);
}

void Trace::Write(const std::string& string, Level level, const char* context)
{
    Write(string.c_str(), level, context);
}

void Trace::Write(const char* text, Level level, const char* context)
{
    WriteInternal(text, level, context);
}
