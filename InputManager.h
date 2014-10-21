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

class InputManager
{
public:
    InputManager();

    bool        IsKeyPressed(MetaKey key) const;
    bool        IsKeyTriggered(MetaKey key) const;

    void        DoKeyDown(const InputKey& args);
    void        DoKeyUp(const InputKey& args);
    void        ReleaseKeys();

    void        Update(float elapsed);
    void        Reset();

private:
    static bool TranslateKey(const InputKey& args, MetaKey* key);

    unsigned    m_keyTriggerFlags;
    unsigned    m_keyFlags;
};
