#pragma once
#include <cstdint>
#include "particle.hpp"
#include "../common/math.hpp"

class LinkConstraint
{
public:
    LinkConstraint() = default;

    LinkConstraint(ParticleRef p_1, ParticleRef p_2)
        : m_particle_1(p_1), m_particle_2(p_2)
    {
        m_distance = MathVec2::length(p_1->m_position - p_2->m_position);
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
        const sf::Vector2f v = p_1.m_position - p_2.m_position;
        const float dist = MathVec2::length(v);
        if (dist > m_distance)
        {
            // Break if the distance is over the given threshold
            m_broken = dist > m_distance * m_max_elongation_ratio;
            const sf::Vector2f n = v / dist;
            const float c = m_distance - dist;
            const sf::Vector2f p = -(c * m_strength) / (p_1.m_mass + p_2.m_mass) * n;
            // Apply position correction
            p_1.move(-p / p_1.m_mass);
            p_2.move(p / p_2.m_mass);
        }
    }

    civ::ID m_id = 0;
    ParticleRef m_particle_1;
    ParticleRef m_particle_2;
    float m_distance = 1.0f;
    float m_strength = 1.0f;
    float m_max_elongation_ratio = 1.5f;
    bool m_broken = false;
};
