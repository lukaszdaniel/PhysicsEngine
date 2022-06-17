#pragma once
#include "engine/physics/physics.hpp"

namespace usr
{
    namespace Utils
    {
        bool isInRadius(const Particle &p, sf::Vector2f center, float radius)
        {
            const sf::Vector2f v = center - p.position;
            return MathVec2::dot(v, v) < radius * radius;
        }

        void applyForceOnCloth(sf::Vector2f position, float radius, sf::Vector2f force, Scene &scene)
        {
            for (Particle &p : scene.m_objects)
            {
                if (isInRadius(p, position, radius))
                {
                    p.forces += force;
                }
            }
        }
    } // namespace Utils
} // namespace usr
