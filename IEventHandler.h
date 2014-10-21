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

class IEventHandler
{
public:
    virtual void OnKeyDown(const InputKey& args) = 0;
    virtual void OnKeyUp(const InputKey& args) = 0;
    virtual void OnMouseMove(const InputMouse& args) = 0;
    virtual void OnMouseButtonDown(const InputMouse& args) = 0;
    virtual void OnMouseButtonUp(const InputMouse& args) = 0;
    virtual void OnMouseWheel(const InputMouse& args) = 0;
    virtual void OnActivate() = 0;
    virtual void OnDeactivate() = 0;
    virtual void OnQuit() = 0;

protected:
    ~IEventHandler() {}
};
