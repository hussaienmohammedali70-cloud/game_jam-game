#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <array>

#include "player.h"
#include "physics.h"
#include "stage.h"
#include "animation.h"
#include "dialog.h"

Player plr;

int main()
{
    sf::RenderTexture darkMask;
    darkMask.create(1920, 1080);

    Physics physics; 
    Stage stage;
    sf::Clock clock;

    sf::BlendMode eraseBlend(
        sf::BlendMode::Factor::Zero,             // Color Source
        sf::BlendMode::Factor::One,              // Color Destination
        sf::BlendMode::Equation::Add,            // Color Equation
        sf::BlendMode::Factor::Zero,             // Alpha Source
        sf::BlendMode::Factor::OneMinusSrcAlpha, // Alpha Destination
        sf::BlendMode::Equation::Add             // Alpha Equation
    );

    // Set up window first so we can cleanly initialize textures
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "stickBANG");
    window.setFramerateLimit(60);

    // Inside main.cpp setup block (Before the while loop)
    Dialog dialogBox;

    // 1. Load the font and check for failures
    if (!dialogBox.init_dialog("font.ttf")) 
    {
        std::cout << "❌ FONT ERROR: Could not find font.ttf in your folder!" << std::endl;
    } 
    else 
    {
        std::cout << "✅ SUCCESS: Dialogue font loaded perfectly!" << std::endl;
    }

    std::vector<std::string> initial_message = { 
        "It's locked from the inside... I need to find another way out." 
    };
    dialogBox.start_dialogue(initial_message);

    if (!stage.load_from_tiled("untitled.tmx")) 
    {    
        std::cout << "❌ CRITICAL: The Tiled map file failed to load!" << std::endl;
    } 
    else 
    {
        std::cout << "✅ SUCCESS: Tiled map loaded into memory!" << std::endl;
    }
    
    stage.init_camera(1920.f, 1080.f);
    
    plr.setTexture(&plr.plr_texture);
    Animation plr_animation(&plr.plr_texture, sf::Vector2u(4, 4), 0.1f);
    plr.setTextureRect(plr_animation.texture_rect);
    
    sf::Texture lightTexture;
    if (lightTexture.loadFromFile("light.png")) {
        lightTexture.setSmooth(false);
    }

    sf::Sprite lightSprite(lightTexture);
    lightSprite.setOrigin(lightTexture.getSize().x / 2.0f, (lightTexture.getSize().y / 2.0f) + 10);
    lightSprite.setScale(10.f, 10.f);

    while(window.isOpen()) 
    {
        float dt = clock.restart().asSeconds();
        sf::Event event;

        // =========================================================================
        // --- ALL EVENTS MUST STAY INSIDE THIS WINDOW POLL LOOP ---
        // =========================================================================
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            // 1. INSTANT TAP CONTROLS (Pressed)
            if(event.type == sf::Event::KeyPressed) 
            {
                if(event.key.code == sf::Keyboard::Space)
                {
                    plr.jump();
                }

                if(event.key.code == sf::Keyboard::L) 
                {
                    plr.dash(plr.velocity);
                }
            }

            // 2. FIXED SYSTEM HOOK: Dialogue controls properly nested inside event queue!
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::E)
                {
                    if (dialogBox.active()) 
                    {
                        dialogBox.advance(); 
                    } 
                    else 
                    {
                        std::vector<std::string> story = {
                            "Hello? Is someone out there...?",
                            "The power went out an hour ago.",
                            "Don't go into the basement. Something is down there."
                        };
                        dialogBox.start_dialogue(story);
                    }
                }
            }
        } // === END OF WINDOW EVENT POLLING ===

        if(plr.is_grounded)
        {
            plr.dash_count = 0;
            plr.can_dash = false;
        }
        else
        {
            plr.can_dash = true;
        }

        plr.move(dt); 
        physics.apply_gravity(plr, dt); 
        plr.update_pos(dt); 

        // Complete Tiled map collisions automation
        for (const auto& platform : stage.level_colliders) {
            physics.collide(plr, platform);
        }

        // Handle animation updates cleanly
        if(plr.is_moving)
        {
            plr_animation.update(0, dt);
        }
        plr.setTextureRect(plr_animation.texture_rect);

        // Update the custom camera scripts using our tracking positions
        stage.update_camera(dt, sf::Vector2f(plr.x, plr.y), plr.direction.x, stage.panic_meter);

        // --- DRAWING PREPARATION (Lighting mask generation) ---
        darkMask.clear(sf::Color(0, 0, 0, 200)); 
        darkMask.setView(stage.get_camera_view());

        // Snap light center position nodes to character metrics
        float player_center_x = plr.x + (plr.width / 2.0f);
        float player_center_y = plr.y + (plr.height / 2.0f);
        lightSprite.setPosition(player_center_x, player_center_y);

        // Carve light channel through dark mask
        darkMask.draw(lightSprite, eraseBlend);
        darkMask.display();

        // --- RENDERING ROUTINE ---
        window.clear(sf::Color::Black); 

        // Apply active player tracking lens
        window.setView(stage.get_camera_view());

        stage.draw_stage(window); 
        
        // Manual sandbox tester platform
        physics.collide(plr, stage.create_platform(window, sf::Color::White, 300.0f, 400.0f, 300.0f, 50.0f));
        
        plr.draw_plr(window); 
        plr.draw_plr_sprite(window);
        
        // Reset view matrix cleanly back to a static flat HUD overlay layer layout
        window.setView(window.getDefaultView());
        
        sf::Sprite darkMaskSprite(darkMask.getTexture());
        window.draw(darkMaskSprite); 
        
        // Render UI dialogue components on top of lighting mask
        dialogBox.draw_dialog(window);

        window.display(); 
    } 
    return 0;
}
