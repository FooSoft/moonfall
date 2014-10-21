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

namespace Trace
{
    enum Level
    {
        LEVEL_INFO,
        LEVEL_WARNING,
        LEVEL_ERROR
    };

    class ITarget
    {
    public:
        virtual void Trace(Level level, const char* text, const char* context) = 0;

    protected:
        ~ITarget() { }
    };

    void AddTarget(ITarget* target, Level threshold = LEVEL_INFO);
    void RemoveTarget(ITarget* target);
    void ClearTargets();

    void Write(const boost::format format, Level level = LEVEL_INFO, const char* context = NULL);
    void Write(const std::string& string, Level level = LEVEL_INFO, const char* context = NULL);
    void Write(const char* text, Level level = LEVEL_INFO, const char* context = NULL);
}

#define TRACE_INFO(text)    Trace::Write(text, Trace::LEVEL_INFO)
#define TRACE_WARNING(text) Trace::Write(text, Trace::LEVEL_WARNING)
#define TRACE_ERROR(text)   Trace::Write(text, Trace::LEVEL_ERROR)
