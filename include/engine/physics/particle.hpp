#pragma once
#include <SFML/System/Vector2.hpp>
#include "../common/index_vector.hpp"

class Particle
{
public:
    Particle() = default;

    explicit Particle(sf::Vector2f pos)
        : m_position(pos), m_position_old(pos)
    {
    }

    void update(float dt)
    {
        if (!m_moving)
            return;
        m_position_old = m_position;
        m_velocity += (m_forces / m_mass) * dt;
        m_position += m_velocity * dt;
    }

    void updateDerivatives(float dt)
    {
        m_velocity = (m_position - m_position_old) / dt;
        m_forces = {};
    }

    void move(sf::Vector2f v)
    {
        if (!m_moving)
            return;
        m_position += v;
    }

    sf::Vector2f position() const
    {
        return m_position;
    }

    float mass() const
    {
        return m_mass;
    }

    void setMoving(bool on)
    {
        m_moving = on;
    }

    sf::Vector2f velocity() const
    {
        return m_velocity;
    }

    sf::Vector2f forces() const
    {
        return m_forces;
    }

    void applyGravity(const sf::Vector2f &gravity)
    {
        m_forces += gravity * m_mass;
    }

    void applyAirFriction(const float &airFriction)
    {
        m_forces -= m_velocity * airFriction;
    }

    void applyForce(const sf::Vector2f &netForce)
    {
        m_forces += netForce;
    }

    void setID(const civ::ID &newID)
    {
        m_id = newID;
    }

private:
    civ::ID m_id = 0;
    float m_mass = 1.0f;
    bool m_moving = true;
    sf::Vector2f m_position;
    sf::Vector2f m_position_old;
    sf::Vector2f m_velocity;
    sf::Vector2f m_forces;
};

using ParticleRef = civ::Ref<Particle>;
