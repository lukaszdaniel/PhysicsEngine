#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <engine/physics/PhysicSolver.hpp>

class Wind
{
public:
    Wind(sf::Vector2f s, sf::Vector2f p, sf::Vector2f force)
        : m_rect(p, s), m_force(force)
    {
    }

    void update(float dt)
    {
        m_rect.left += 1.0f * m_force.x * dt;
        // m_rect.top += m_force.y * dt;
    }

    sf::FloatRect &rect()
    {
        return m_rect;
    }

    sf::Vector2f force()
    {
        return m_force;
    }

private:
    sf::FloatRect m_rect;
    sf::Vector2f m_force;
};

class WindManager
{
public:
    explicit WindManager(uint32_t width, unsigned int num_waves)
        : m_world_width(width)
    {
        m_wind_waves.reserve(num_waves);
    }

    void update(Scene &scene, float dt)
    {
        for (Wind &w : m_wind_waves)
        {
            w.update(dt);
            for (Particle &p : scene.objects())
            {
                if (w.rect().contains(p.position()))
                {
                    p.applyForce(1.0f * w.force() / dt);
                }
            }

            if (w.rect().left > m_world_width)
            {
                w.rect().left = -w.rect().width;
            }
        }
    }

    std::vector<Wind> &winds()
    {
        return m_wind_waves;
    }

private:
    std::vector<Wind> m_wind_waves;
    float m_world_width;
};
