#pragma once
#include <SFML/Graphics.hpp>
#include "engine/physics/physics.hpp"
#include "engine/window_context_handler.hpp"

class Renderer
{
public:
    explicit Renderer(Scene &s)
        : m_scene(s), m_va(sf::Lines)
    {
    }

    void updateVA()
    {
        const uint32_t links_count = to<uint32_t>(m_scene.constraints().size());
        m_va.resize(2 * links_count);
        for (uint32_t i(0); i < links_count; ++i)
        {
            LinkConstraint &current_link = m_scene.constraints().data[i];
            m_va[2 * i].position = current_link.particle_1()->position();
            m_va[2 * i + 1].position = current_link.particle_2()->position();
        }
    }

    void render(RenderContext &context)
    {
        updateVA();
        context.draw(m_va);
    }

private:
    Scene &m_scene;
    sf::VertexArray m_va;
};
