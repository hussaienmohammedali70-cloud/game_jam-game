#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

#include "state_enums.h"
#include "player.h"
#include "stage.h"
#include "physics.h"
#include "animation.h"
#include "dialog.h"

class GameManager
{
private:
    
    sf::Text intro_title;
    sf::Font intro_font;

    GameState current_state;
    sf::RenderWindow window;
    sf::Clock clock;

    Physics physics; 
    Stage stage;
    Dialog dialogBox;
    Animation* plr_animation;

    sf::RenderTexture darkMask;
    sf::Texture lightTexture;
    sf::Sprite lightSprite;
    sf::BlendMode eraseBlend;

    sf::Font menu_font;
    sf::Text pause_text;
    sf::Text intro_title;

    // 👇 AUDIO SUBSYSTEM MANAGERS 👇
    sf::Music bg_music;        // Streams long background loops (ambience/chase)
    sf::SoundBuffer sound_buff; // Buffers short effects files
    sf::Sound sfx_sound;

    void init_assets();
    void handle_events(sf::Event& event);
    void update(float dt);
    void render();

    void draw_gameplay();
    void draw_pause();
    void draw_intro();

    // Map changer internal pipeline helper
    void switch_map(const std::string& new_map_path, const std::string& new_audio_path);

public:
    GameManager();
    ~GameManager();
    void run();
    void trigger_cutscene(const std::vector<std::string>& script, sf::Vector2f camera_focus_pos);
};
