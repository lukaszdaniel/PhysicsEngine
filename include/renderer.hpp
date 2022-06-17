#pragma once
#include <SFML/Graphics.hpp>
#include "engine/physics/physics.hpp"
#include "engine/window_context_handler.hpp"

struct Renderer
{
    Scene &m_scene;
    sf::VertexArray va;

    explicit Renderer(Scene &s)
        : m_scene(s), va(sf::Lines)
    {
    }

    void updateVA()
    {
        const uint32_t links_count = to<uint32_t>(m_scene.constraints.size());
        va.resize(2 * links_count);
        for (uint32_t i(0); i < links_count; ++i)
        {
            LinkConstraint &current_link = m_scene.constraints.data[i];
            va[2 * i].position = current_link.particle_1->position;
            va[2 * i + 1].position = current_link.particle_2->position;
        }
    }

    void render(RenderContext &context)
    {
        updateVA();
        context.draw(va);
    }
};
