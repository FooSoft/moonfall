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

class ICameraTarget;

class Camera
{
public:
    void                                Update(float elapsed);

    void                                SetTarget(const boost::shared_ptr<ICameraTarget>& target);
    void                                SetTarget(const Vector2i& target);
    Vector2i                            GetTarget() const;
    Rect4i                              Frame(const Rect4i& viewport) const;
    Vector2i                            Project(const Rect4i& viewport, const Vector2i& point) const;

private:
    boost::shared_ptr<ICameraTarget>    m_targetDynamic;
    Vector2i                            m_targetStatic;
};
