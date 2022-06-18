#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <engine/common/index_vector.hpp>
#include <engine/common/utils.hpp>
#include <engine/physics/Scene.hpp>

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
