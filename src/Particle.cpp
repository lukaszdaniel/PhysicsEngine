#include <engine/physics/Particle.hpp>
#include <engine/common/math.hpp>
#include <engine/physics/PhysicSolver.hpp>

bool isInRadius(const Particle &p, sf::Vector2f center, float radius)
{
    const sf::Vector2f v = center - p.position();
    return MathVec2::dot(v, v) < radius * radius;
}

void applyForceOnCloth(sf::Vector2f position, float radius, sf::Vector2f force, Scene &scene)
{
    for (Particle &p : scene.objects())
    {
        if (isInRadius(p, position, radius))
        {
            p.applyForce(force);
        }
    }
}