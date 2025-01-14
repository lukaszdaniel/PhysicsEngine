#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <engine/common/index_vector.hpp>
#include <engine/common/utils.hpp>
#include <engine/physics/Constraints.hpp>

class Scene
{
public:
    Scene(uint32_t window_width = 1920, uint32_t window_height = 1080)
    {
        initialize(window_width, window_height);
    }

    void initialize(uint32_t window_width, uint32_t window_height)
    {
        // Initialize the cloth
        const uint32_t cloth_width = 75;
        const uint32_t cloth_height = 50 + 0 * window_height;
        const float links_length = 20.0f;
        const float start_x = (window_width - (cloth_width - 1) * links_length) * 0.5f;
        for (uint32_t y = 0; y < cloth_height; ++y)
        {
            // This is just an arbitrary formula to make the top links stronger since
            // they are under bigger stress
            const float max_elongation = 1.2f * (2.0f - to<float>(y) / float(cloth_height));
            for (uint32_t x = 0; x < cloth_width; ++x)
            {
                const civ::ID id = addParticle(sf::Vector2f(start_x + to<float>(x) * links_length, to<float>(y) * links_length));
                // Add left link if there is a particle on the left
                if (x > 0)
                {
                    addLink(id - 1, id, max_elongation * 0.9f);
                }
                // Add top link if there is a particle on the top
                if (y > 0)
                {
                    addLink(id - cloth_width, id, max_elongation);
                }
                else
                {
                    // If not, pin the particle
                    objects(id).setMoving(false);
                }
            }
        }
    }

    void preUpdate()
    {
        removeBrokenLinks();
    }

    void postUpdate()
    {
    }

    void removeBrokenLinks()
    {
        m_constraints.remove_if([](const LinkConstraint &c)
                                { return !c.isValid(); });
    }

    void applyGravity(const sf::Vector2f &gravity)
    {
        for (Particle &p : m_objects)
        {
            p.applyGravity(gravity);
        }
    }

    void applyAirFriction(const float &friction_coef)
    {
        for (Particle &p : m_objects)
        {
            p.applyAirFriction(friction_coef);
        }
    }

    void updatePositions(float dt)
    {
        for (Particle &p : m_objects)
        {
            p.update(dt);
        }
    }

    void updateDerivatives(float dt)
    {
        for (Particle &p : m_objects)
        {
            p.updateDerivatives(dt);
        }
    }

    void solveConstraints()
    {
        for (LinkConstraint &l : m_constraints)
        {
            l.solve();
        }
    }

    CIVector<Particle> &objects()
    {
        return m_objects;
    }

    Particle &objects(const civ::ID id)
    {
        return m_objects[id];
    }

    civ::ID addParticle(sf::Vector2f position)
    {
        const civ::ID new_particle_id = m_objects.emplace_back(position);
        m_objects[new_particle_id].setID(new_particle_id);
        return new_particle_id;
    }

    void addLink(civ::ID particle_1, civ::ID particle_2, float max_elongation_ratio = 1.5f)
    {
        const civ::ID link_id = m_constraints.emplace_back(m_objects.getRef(particle_1), m_objects.getRef(particle_2));
        m_constraints[link_id].setID(link_id);
        m_constraints[link_id].setMaxElongation(max_elongation_ratio);
    }

    CIVector<LinkConstraint> &constraints()
    {
        return m_constraints;
    }

    // void map(const std::function<void(Particle &)> &callback)
    // {
    //     for (Particle &p : m_objects)
    //     {
    //         callback(p);
    //     }
    // }

private:
    CIVector<Particle> m_objects;
    CIVector<LinkConstraint> m_constraints;
};