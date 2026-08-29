#include "game_manager.h"
#include <iostream>

Player plr; 

GameManager::GameManager() : current_state(GameState::Gameplay), plr_animation(nullptr), camera_should_pan(false)
{
    window.create(sf::VideoMode(1920, 1080), "Black Sun");
    window.setFramerateLimit(60);
    init_assets();
}

GameManager::~GameManager()
{
    delete plr_animation;
}

void GameManager::init_assets()
{
    // 1. Setup Alpha Subtractive Flashlight parameters
    eraseBlend.colorSrcFactor = sf::BlendMode::Factor::Zero;
    eraseBlend.colorDstFactor = sf::BlendMode::Factor::One;
    eraseBlend.colorEquation  = sf::BlendMode::Equation::Add;
    eraseBlend.alphaSrcFactor = sf::BlendMode::Factor::Zero;
    eraseBlend.alphaDstFactor = sf::BlendMode::Factor::OneMinusSrcAlpha;
    eraseBlend.alphaEquation  = sf::BlendMode::Equation::Add;

    darkMask.create(1920, 1080);

    if (lightTexture.loadFromFile("light.png")) 
    {
        lightTexture.setSmooth(false);
    }

    lightSprite.setTexture(lightTexture);
    lightSprite.setOrigin(lightTexture.getSize().x / 2.0f, (lightTexture.getSize().y / 2.0f) + 10);
    lightSprite.setScale(10.f, 10.f);

    if (!dialogBox.init_dialog("font.ttf")) 
    {
        std::cout << "FONT ERROR: Could not find font.ttf!" << std::endl;
    }
    
    // Optional starter baseline script hook
    std::vector<std::string> initial_message = { "It's locked from the inside... I need to find another way out." };
    dialogBox.start_dialogue(initial_message);

    if (!stage.load_from_tiled("untitled.tmx")) {
        std::cout << "CRITICAL: The Tiled map file failed to load!" << std::endl;
    }

    stage.init_camera(1920.f, 1080.f);

    plr.setTexture(&plr.plr_texture);
    plr_animation = new Animation(&plr.plr_texture, sf::Vector2u(4, 4));
    plr.setTextureRect(plr_animation->texture_rect);

    if (menu_font.loadFromFile("font.ttf")) 
    {
        pause_text.setFont(menu_font);
        pause_text.setString("Pause - press P to continue");
        pause_text.setCharacterSize(40);
        pause_text.setFillColor(sf::Color::Yellow);
        pause_text.setPosition(1920.f / 2.f - 350.f, 500.f);
    }




    if (intro_font.loadFromFile("font.ttf")) {
        intro_title.setFont(intro_font);
        intro_title.setString("BLACK SUN");
        intro_title.setCharacterSize(80);
        intro_title.setFillColor(sf::Color(139, 0, 0)); // Dark Horror Red
        intro_title.setPosition(1920.f / 2.f - 220.f, 400.f);
    }


    std::vector<std::string> intro_text = {
        "August 1996... I shouldn't have come back to this place.",
        "The power grid went completely dead hours ago.",
        "Press E to continue..."
    };
    dialogBox.start_dialogue(intro_text);
}




void GameManager::switch_map(const std::string& new_map_path, const std::string& new_audio_path)
{
  
    if (stage.load_from_tiled(new_map_path)) 
    {



        if (!new_audio_path.empty()) 
        {
            bg_music.stop();
            // Looks for the file path name passed by the trigger (e.g. "cave_wind.ogg")
            if (bg_music.openFromFile(new_audio_path)) 
            {
                bg_music.setLoop(true);
                bg_music.play();
            }
            else
            {
                
            }
        }

        // 3. TELEPORT PLAYER COORDINATES
        // Safely resets player position back to the beginning layout bounds of the new canvas
        plr.x = 200.0f;
        plr.y = 200.0f;
        plr.velocity = sf::Vector2f(0.f, 0.f);
        plr.update_pos(0.f);
    }
    else
    {
        std::cerr << "❌ FAILED TO TELEPORT: Could not find map file: " << new_map_path << std::endl;
    }
}



void GameManager::handle_events(sf::Event& event)
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        switch (current_state)
        {
            case GameState::Gameplay:
                // A. TAP PRESSED CONTROLS
                if (event.type == sf::Event::KeyPressed) 
                {
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        plr.jump();
                        // plr.is_jumping = true; (Uncomment if your player.h tracks this flag!)
                    }

                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }

                    if (event.key.code == sf::Keyboard::L) 
                    {
                        plr.dash(plr.velocity);
                    }
                    
                    if (event.key.code == sf::Keyboard::P) 
                    {
                        current_state = GameState::Paused;
                    }
                }

                // B. TAP RELEASED CONTROLS (Safely moved outside KeyPressed wrapper)
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
                break;

            case GameState::Cutscene:
                if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::E) { 
                    if (dialogBox.active()) {
                        dialogBox.advance();
                        
                        if (!dialogBox.active()) {
                            current_state = GameState::Gameplay; 
                            camera_should_pan = false;
                        }
                    }
                }
                break;

            case GameState::Paused:
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                    current_state = GameState::Gameplay;
                }
                break;
        }
    }
}

void GameManager::update(float dt)
{
    for (auto& prop : stage.active_props) {
        if (prop.is_playing) {
            prop.anim_timer += dt;
            if (prop.anim_timer >= prop.anim_speed) {
                prop.anim_timer -= prop.anim_speed;
                prop.current_frame++;
                
                if (prop.current_frame < prop.total_frames) {
                    // Shift the texture rectangle window selection horizontally to the next animation cell frame
                    prop.sprite.setTextureRect(sf::IntRect(prop.current_frame * prop.frame_size.x, 0, prop.frame_size.x, prop.frame_size.y));
                } else {
                    // Animation hit the final frame! Freeze it on the end state or loop it
                    prop.is_playing = false; 
                }
            }
        }
    }

    
    dialogBox.update(dt); // update teh typewrite text effect //
    if (current_state == GameState::Cutscene)
    {
        if (camera_should_pan) {
            sf::View active_view = stage.get_camera_view();
            sf::Vector2f current_center = active_view.getCenter();
            
            float pan_speed = 3.0f; 
            current_center.x += (cutscene_camera_target.x - current_center.x) * pan_speed * dt; 
            current_center.y += (cutscene_camera_target.y - current_center.y) * pan_speed * dt;
        }
    }

    else if (current_state == GameState::Gameplay) 
    {
        if (plr.is_grounded) 
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

        for (const auto& platform : stage.level_colliders) 
        {
            physics.collide(plr, platform);
        }

        sf::FloatRect player_bounds = plr.get_bounds();

        for (auto& trigger : stage.map_triggers) 
        {
            if (player_bounds.intersects(trigger.bounds))
            {
                if (!trigger.text_content.empty()) 
                {
                    std::vector<std::string> lines;
                    std::string current_line = "";

                    for (char c : trigger.text_content) 
                    {
                        if (c == '/') 
                        {
                            lines.push_back(current_line);
                            current_line = "";
                        } 
                        else
                        {
                            current_line += c;
                        }
                    }
                    lines.push_back(current_line);

                    if (!lines.empty() && lines[0].find("[CUTSCENE]") != std::string::npos) 
                    {
                        lines[0] = lines[0].substr(10); 
                        trigger_cutscene(lines, sf::Vector2f(trigger.bounds.left + trigger.bounds.width / 2.f, 
                                                             trigger.bounds.top + trigger.bounds.height / 2.f));
                    } 
                    else {
                        dialogBox.start_dialogue(lines);
                    }
                    trigger.text_content = ""; 
                }
            }
        }

        if (plr.is_moving) 
        {
            plr_animation->update(0, 4, 0.2, dt);
        } 
        else
        {
            plr_animation->update(1, 2, 0.7, dt);
        }

        plr.setTextureRect(plr_animation->texture_rect);
        stage.update_camera(dt, sf::Vector2f(plr.x, plr.y), plr.direction.x, stage.panic_meter);

        darkMask.clear(sf::Color(0, 0, 0, 200));
        darkMask.setView(stage.get_camera_view());

        float player_center_x = plr.x + (plr.width / 2.0f);
        float player_center_y = plr.y + (plr.height / 2.0f);

        lightSprite.setPosition(player_center_x, player_center_y);
        darkMask.draw(lightSprite, eraseBlend);
        darkMask.display();
    }
}

void GameManager::render()
{
    window.clear(sf::Color::Black);

    switch (current_state)
    {
        case GameState::Gameplay:
            draw_gameplay();
            break;

        case GameState::Cutscene:
            draw_gameplay(); 
            break;

        case GameState::Paused:
            draw_gameplay(); 
            draw_pause();
            break;
    }

    window.display();
}

void GameManager::draw_gameplay()
{
    window.setView(stage.get_camera_view());
    stage.draw_stage(window);
    
    plr.draw_plr(window);
    plr.draw_plr_sprite(window);

    window.setView(window.getDefaultView());
    sf::Sprite darkMaskSprite(darkMask.getTexture());
    window.draw(darkMaskSprite); 
    dialogBox.draw_dialog(window);
}

void GameManager::draw_pause()
{
    sf::RectangleShape dim_overlay(sf::Vector2f(1920.f, 1080.f));
    dim_overlay.setFillColor(sf::Color(0, 0, 0, 150));
    
    window.setView(window.getDefaultView());
    window.draw(dim_overlay);
    window.draw(pause_text);
}

void GameManager::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        
        handle_events(event);
        update(dt);
        render();
    }
}

void GameManager::trigger_cutscene(const std::vector<std::string>& script, sf::Vector2f camera_focus_pos)
{
    current_state = GameState::Cutscene;
    cutscene_camera_target = camera_focus_pos;
    camera_should_pan = true;
    dialogBox.start_dialogue(script);
}
