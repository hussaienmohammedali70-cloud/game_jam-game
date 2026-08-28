#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Stage
{
private:
    sf::RectangleShape stage_body;
    std::vector<sf::VertexArray> visual_layers;
    sf::Texture tileset_texture;

    // Camera structural variables
    sf::View camera;
    sf::Vector2f camera_target;
    sf::Vector2f current_camera_pos;

public:
    float panic_meter;           

    std::vector<sf::FloatRect> level_colliders; 
    sf::RectangleShape panic_bar_bg;
    sf::RectangleShape panic_bar_fg;
    sf::Text subtext_prompt;

    float width, height, x, y;

    Stage();

    bool load_from_tiled(const std::string& tmx_file_path);
    void draw_stage(sf::RenderWindow& window);
    void setup_stage();
    
    // --- CAMERA FUNCTIONS REGISTRATION ---
    void init_camera(float window_w, float window_h);
    void update_camera(float dt, const sf::Vector2f& player_pos, int player_dir_x, float panic_level);
    const sf::View& get_camera_view() const { return camera; }
    
    sf::FloatRect get_bounds();
    sf::FloatRect create_platform(sf::RenderWindow& window, sf::Color color, float x, float y, float w, float h); 
};
