#include <engine/WindowContextHandler.hpp>
#include <engine/physics/PhysicSolver.hpp>
#include <Renderer.hpp>
#include <WindManager.hpp>
#include <engine/physics/Particle.hpp>

int main()
{
    const uint32_t window_width = 1920;
    const uint32_t window_height = 1080;
    WindowContextHandler app("Cloth", sf::Vector2u(window_width, window_height), sf::Style::Default);

    // Initialize the scene
    Scene scene(window_width, window_height);

    // Initialize solver and renderer
    PhysicSolver solver(scene);
    Renderer renderer(scene);

    sf::Vector2f last_mouse_position;
    bool dragging = false;
    bool erasing = false;
    bool active_wind = false;

    // Add events callback for mouse control
    app.getEventManager().addMousePressedCallback(sf::Mouse::Right, [&](const sf::Event &)
                                                  { dragging = true; last_mouse_position = app.getWorldMousePosition(); });
    app.getEventManager().addMouseReleasedCallback(sf::Mouse::Right, [&](const sf::Event &)
                                                   { dragging = false; });
    app.getEventManager().addMousePressedCallback(sf::Mouse::Middle, [&](const sf::Event &)
                                                  { erasing = true; });
    app.getEventManager().addMouseReleasedCallback(sf::Mouse::Middle, [&](const sf::Event &)
                                                   { erasing = false; });

    app.getEventManager().addKeyPressedCallback(sf::Keyboard::W, [&](const sf::Event &)
                                                { active_wind = !active_wind; });

    // Add 2 wind waves
    WindManager wind(window_width, 2 /* number of wind waves */);
    wind.winds().push_back(Wind(sf::Vector2f(100.0f, window_height), sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1000.0f, 0.0f)));
    wind.winds().push_back(Wind(sf::Vector2f(20.0f, window_height), sf::Vector2f(0.0f, 0.0f), sf::Vector2f(3000.0f, 0.0f)));

    // Main loop
    constexpr float dt = 1.0f / 60.0f;
    while (app.run())
    {
        // Get the mouse coord in the world space, to allow proper control even with modified viewport
        const sf::Vector2f mouse_position = app.getWorldMousePosition();

        if (dragging)
        {
            // Apply a force on the particles in the direction of the mouse's movement
            const sf::Vector2f mouse_speed = (mouse_position - last_mouse_position) /* / 1.0 */;
            last_mouse_position = mouse_position;
            applyForceOnCloth(mouse_position, 100.0f, mouse_speed * 8000.0f, scene);
        }

        if (erasing)
        {
            // Delete all nodes that are in the range of the mouse
            scene.objects().remove_if([&](const Particle &p)
                                      { return isInRadius(p, mouse_position, 10.0f); });
        }

        // Update physics
        if (active_wind)
            wind.update(scene, dt);
        solver.update(dt);

        // Render the scene
        RenderContext &render_context = app.getRenderContext();
        render_context.clear();
        renderer.render(render_context);
        render_context.display();
    }

    return 0;
}
