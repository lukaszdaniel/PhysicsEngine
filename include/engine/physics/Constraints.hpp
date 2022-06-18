#pragma once
#include <cstdint>
#include <engine/physics/Particle.hpp>
#include <engine/common/math.hpp>

class LinkConstraint
{
public:
    LinkConstraint() = default;

    LinkConstraint(ParticleRef p_1, ParticleRef p_2)
        : m_particle_1(p_1), m_particle_2(p_2)
    {
        m_distance = MathVec2::length(p_1->position() - p_2->position());
    }

    [[nodiscard]] bool isValid() const
    {
        return m_particle_2 && m_particle_1 && !m_broken;
    }

    void solve()
    {
        if (!isValid())
        {
            return;
        }
        Particle &p_1 = *m_particle_1;
        Particle &p_2 = *m_particle_2;
        const sf::Vector2f v = p_1.position() - p_2.position();
        const float dist = MathVec2::length(v);
        if (dist > m_distance)
        {
            // Break if the distance is over the given threshold
            m_broken = dist > m_distance * m_max_elongation_ratio;
            const sf::Vector2f n = v / dist;
            const float c = m_distance - dist;
            const sf::Vector2f p = -(c * m_strength) / (p_1.mass() + p_2.mass()) * n;
            // Apply position correction
            p_1.move(-p / p_1.mass());
            p_2.move(p / p_2.mass());
        }
    }

    void setMaxElongation(float val)
    {
        m_max_elongation_ratio = val;
    }

    auto particle_1()
    {
        return m_particle_1;
    }

    auto particle_2()
    {
        return m_particle_2;
    }

    void setID(civ::ID newID)
    {
        m_id = newID;
    }

private:
    civ::ID m_id = 0;
    ParticleRef m_particle_1;
    ParticleRef m_particle_2;
    float m_distance = 1.0f;
    float m_strength = 1.0f;
    float m_max_elongation_ratio = 1.5f;
    bool m_broken = false;
};
