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

class ActorManager;
class ActorProperty;

class Actor : private boost::noncopyable
{
public:
    Actor(const char* alias, const char* name);

    void                                Render(const boost::shared_ptr<Surface>& target);
    void                                Update(float elapsed);

    void                                SetPosition(const Vector2i& position);
    Vector2i                            GetPosition() const;
    void                                SetPositionSoft(const Vector2f& position);
    Vector2f                            GetPositionSoft() const;
    void                                SetLayer(ActorLayer layer);
    ActorLayer                          GetLayer() const;
    void                                SetDynamic(bool dynamic);
    bool                                IsDynamic() const;

    void                                SetName(const char* name);
    const char*                         GetName() const;
    Token                               GetId() const;
    const char*                         GetAlias() const;

    template <typename T>
    boost::weak_ptr<T> AddProperty()
    {
        ASSERT(T::GetStaticType() < ARRAY_SIZE(m_properties));
        boost::shared_ptr<ActorProperty>& property = m_properties[T::GetStaticType()];
        if (!property)
        {
            property = boost::shared_ptr<T>(new T(this));
        }
        return GetProperty<T>();
    }

    template <typename T>
    void RemoveProperty()
    {
        ASSERT(T::GetStaticType() < ARRAY_SIZE(m_properties));
        m_properties[T::GetStaticType()].reset();
    }

    template <typename T>
    boost::weak_ptr<const T> GetProperty() const
    {
        ASSERT(T::GetStaticType() < ARRAY_SIZE(m_properties));
        return boost::static_pointer_cast<T, ActorProperty>(m_properties[T::GetStaticType()]);
    }

    template <typename T>
    boost::weak_ptr<T> GetProperty()
    {
        ASSERT(T::GetStaticType() < ARRAY_SIZE(m_properties));
        return boost::static_pointer_cast<T, ActorProperty>(m_properties[T::GetStaticType()]);
    }

    bool HasProperty(ActorPropertyType property) const
    {
        ASSERT(property < ARRAY_SIZE(m_properties));
        return m_properties[property];
    }

    void                                SetOwner(ActorManager* owner);
    const ActorManager*                 GetOwner() const;
    ActorManager*                       GetOwner();

    void                                GetShape(ActorShapeType type, int* data, Vector2i* position, Vector2i* size) const;
    void                                SetShape(ActorShapeType type, int data, const Vector2i& position, const Vector2i& size);
    ActorShape                          GetShape(ActorShapeType type) const;
    void                                SetShape(ActorShapeType type, const ActorShape& shape);
    void                                EnableShape(ActorShapeType type);
    void                                DisableShape(ActorShapeType type);
    bool                                IsShapeEnabled(ActorShapeType type) const;

private:
    void                                RenderShapes(const boost::shared_ptr<Surface>& target);
    static Token                        RegisterId();
    void                                SetId(Token id);

    boost::shared_ptr<ActorProperty>    m_properties[ACTOR_PROPERTY_TYPE_COUNT];
    ActorShape                          m_shapes[ACTOR_SHAPE_TYPE_COUNT];
    ActorManager*                       m_owner;
    bool                                m_dynamic;
    ActorLayer                          m_layer;
    Vector2f                            m_position;
    std::string                         m_alias;
    std::string                         m_name;
    Token                               m_id;
    boost::shared_ptr<Surface>          m_shapeSurface;
};
