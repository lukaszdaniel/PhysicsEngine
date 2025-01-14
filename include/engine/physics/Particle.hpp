#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <engine/common/index_vector.hpp>

class Scene;

class Particle : public sf::CircleShape
{
public:
    Particle() = default;

    explicit Particle(sf::Vector2f pos)
        : sf::CircleShape(2), m_position(pos), m_position_old(pos)
    {
        setPosition(m_position);
        setFillColor(sf::Color(0x89ABE3FF));
    }

    void update(float dt)
    {
        if (!m_moving)
            return;
        m_position_old = m_position;
        m_velocity += (m_forces / m_mass) * dt;
        m_position += m_velocity * dt;
        setPosition(m_position);
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

bool isInRadius(const Particle &p, sf::Vector2f center, float radius);
void applyForceOnCloth(sf::Vector2f position, float radius, sf::Vector2f force, Scene &scene);