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
#include "WindowDialoguePanel.h"
#include "WindowText.h"
#include "Surface.h"
#include "Sprite.h"

void WindowDialoguePanel::SetPanel(const char* text, const char* textFontAlias, const char* title, const char* titleFontAlias, const char* spriteAlias, int padding, int maxWidth)
{
    ClearChildren();

    if (spriteAlias != NULL)
    {
        m_sprite = System::LoadSprite(spriteAlias);
    }
    else
    {
        m_sprite.reset();
    }

    if (maxWidth == 0)
    {
        maxWidth = GetSize().x;
    }

    boost::shared_ptr<WindowText> titleWindow;
    if (title != NULL && maxWidth > 0)
    {
        titleWindow = boost::shared_ptr<WindowText>(new WindowText());
        titleWindow->SetText(title, titleFontAlias, maxWidth);
        titleWindow->SetSize(titleWindow->GetDesiredSize());

        if (m_sprite && m_sprite->GetSize().x > titleWindow->GetSize().x)
        {
            titleWindow->SetPosition(Vector2i((m_sprite->GetSize().x - titleWindow->GetSize().x) / 2, m_sprite->GetSize().y + padding));
        }
        else
        {
            titleWindow->SetPosition(Vector2i(0, (m_sprite ? m_sprite->GetSize().y + padding : 0)));
        }

        AddChild(titleWindow);
    }

    boost::shared_ptr<WindowText> textWindow;
    if (text != NULL)
    {
        const int titleWidth = titleWindow ? titleWindow->GetDesiredSize().x : 0;
        const int spriteWidth = m_sprite ? m_sprite->GetSize().x : 0;
        const int offsetWidth = MAX(titleWidth, spriteWidth) + ((titleWindow || m_sprite) ? padding : 0);
        const int textWidth = maxWidth - offsetWidth;

        if (textWidth > 0)
        {
            textWindow = boost::shared_ptr<WindowText>(new WindowText());
            textWindow->SetText(text, textFontAlias, textWidth);
            textWindow->SetPosition(Vector2i(offsetWidth, 0));
            textWindow->SetSize(textWindow->GetDesiredSize());
            AddChild(textWindow);
        }
    }

    const Vector2i spriteSize = m_sprite ? m_sprite->GetSize() : Vector2i(0, 0);
    const Vector2i titleSize = titleWindow ? titleWindow->GetDesiredSize() : Vector2i(0, 0);
    const Vector2i textSize = textWindow ? textWindow->GetDesiredSize() : Vector2i(0, 0);

    m_desiredSize = Vector2i(
                        MAX(titleSize.x, spriteSize.x) + textSize.x + (((m_sprite || titleWindow) && textWindow) ? padding : 0),
                        MAX(titleSize.y + spriteSize.y, textSize.y) + ((m_sprite && titleWindow) ? padding : 0)
                    );

    if (m_sprite && titleWindow && titleWindow->GetSize().x > m_sprite->GetSize().x)
    {
        m_spritePosition = Vector2i((titleWindow->GetSize().x - m_sprite->GetSize().x) / 2, 0);
    }
    else
    {
        m_spritePosition = Vector2i(0, 0);
    }
}

Vector2i WindowDialoguePanel::GetDesiredSize() const
{
    return m_desiredSize;
}

void WindowDialoguePanel::OnRender(const boost::shared_ptr<Surface>& target)
{
    Window::OnRender(target);
    if (m_sprite)
    {
        target->SetTranslation(m_spritePosition);
        m_sprite->Render(target);
    }
}
