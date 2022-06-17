#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>
#include "engine/common/index_vector.hpp"
#include "engine/common/utils.hpp"
#include "constraints.hpp"

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
                    m_objects[id].m_moving = false;
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
            p.m_forces += gravity * p.m_mass;
        }
    }

    void applyAirFriction(const float &friction_coef)
    {
        for (Particle &p : m_objects)
        {
            p.m_forces -= p.m_velocity * friction_coef;
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
        const civ::ID particle_id = m_objects.emplace_back(position);
        m_objects[particle_id].m_id = particle_id;
        return particle_id;
    }

    void addLink(civ::ID particle_1, civ::ID particle_2, float max_elongation_ratio = 1.5f)
    {
        const civ::ID link_id = m_constraints.emplace_back(m_objects.getRef(particle_1), m_objects.getRef(particle_2));
        m_constraints[link_id].m_id = link_id;
        m_constraints[link_id].m_max_elongation_ratio = max_elongation_ratio;
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

class PhysicSolver
{
public:
    PhysicSolver(Scene &scene, uint32_t n_iter = 1, uint32_t n_subs = 16)
        : m_scene(scene), m_solver_iterations(n_iter), m_sub_steps(n_subs)
    {
    }

    void update(float dt)
    {
        const float sub_step_dt = dt / to<float>(m_sub_steps);
        preUpdate();

        for (uint32_t i = m_sub_steps; i > 0; --i)
        {
            applyGravity();
            applyAirFriction();
            updatePositions(sub_step_dt);
            solveConstraints();
            updateDerivatives(sub_step_dt);
        }

        postUpdate();
    }

    void preUpdate()
    {
        m_scene.preUpdate();
    }

    void postUpdate()
    {
        m_scene.postUpdate();
    }

    void applyGravity()
    {
        const sf::Vector2f gravity(0.0f, 1500.0f);
        m_scene.applyGravity(gravity);
    }

    void applyAirFriction()
    {
        constexpr float friction_coef = 0.5f;
        m_scene.applyAirFriction(friction_coef);
    }

    void updatePositions(float dt)
    {
        m_scene.updatePositions(dt);
    }

    void updateDerivatives(float dt)
    {
        m_scene.updateDerivatives(dt);
    }

    void solveConstraints()
    {
        for (uint32_t i = m_solver_iterations; i > 0; --i)
        {
            m_scene.solveConstraints();
        }
    }

    Scene &scene()
    {
        return m_scene;
    }

private:
    Scene &m_scene;

    // Simulator iterations count
    uint32_t m_solver_iterations;
    uint32_t m_sub_steps;
};
