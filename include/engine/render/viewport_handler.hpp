#pragma once
#include <SFML/Graphics.hpp>

class ViewportHandler
{
public:
    class State
    {
    public:
        State(sf::Vector2f render_size, const float base_zoom = 1.0f)
            : m_center(render_size.x * 0.5f, render_size.y * 0.5f), m_offset(m_center / base_zoom), m_zoom(base_zoom), m_clicking(false)
        {
        }

        void updateState()
        {
            const float z = m_zoom;
            m_transform = sf::Transform::Identity;
            m_transform.translate(m_center);
            m_transform.scale(z, z);
            m_transform.translate(-m_offset);
        }

        void updateMousePosition(sf::Vector2f new_position)
        {
            m_mouse_position = new_position;
            const sf::Vector2f pos(static_cast<float>(new_position.x), static_cast<float>(new_position.y));
            m_mouse_world_position = m_offset + (pos - m_center) / m_zoom;
        }

        sf::Vector2f m_center;
        sf::Vector2f m_offset;
        float m_zoom;
        bool m_clicking;
        sf::Vector2f m_mouse_position;
        sf::Vector2f m_mouse_world_position;
        sf::Transform m_transform;
    };

    ViewportHandler(sf::Vector2f size)
        : m_state(size)
    {
        m_state.updateState();
    }

    void addOffset(sf::Vector2f v)
    {
        m_state.m_offset += v / m_state.m_zoom;
        m_state.updateState();
    }

    void zoom(float f)
    {
        m_state.m_zoom *= f;
        m_state.updateState();
    }

    void wheelZoom(float w)
    {
        if (w)
        {
            const float zoom_amount = 1.2f;
            const float delta = w > 0 ? zoom_amount : 1.0f / zoom_amount;
            zoom(delta);
        }
    }

    void reset()
    {
        m_state.m_zoom = 1.0f;
        setFocus(m_state.m_center);
    }

    const sf::Transform &getTransform() const
    {
        return m_state.m_transform;
    }

    void click(sf::Vector2f relative_click_position)
    {
        m_state.m_mouse_position = relative_click_position;
        m_state.m_clicking = true;
    }

    void unclick()
    {
        m_state.m_clicking = false;
    }

    void setMousePosition(sf::Vector2f new_mouse_position)
    {
        if (m_state.m_clicking)
        {
            addOffset(m_state.m_mouse_position - new_mouse_position);
        }
        m_state.updateMousePosition(new_mouse_position);
    }

    void setFocus(sf::Vector2f focus_position)
    {
        m_state.m_offset = focus_position;
        m_state.updateState();
    }

    void setZoom(float new_zoom)
    {
        m_state.m_zoom = new_zoom;
        m_state.updateState();
    }

    sf::Vector2f getMouseWorldPosition() const
    {
        return m_state.m_mouse_world_position;
    }

    sf::Vector2f getScreenCoords(sf::Vector2f world_pos) const
    {
        return m_state.m_transform.transformPoint(world_pos);
    }

private:
    State m_state;
};
