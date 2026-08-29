#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "stage.h"
#include "player.h"

extern Player plr;

Stage::Stage() 
{
    x = 100.0f;
    y = 900.0f;
    width = 1700.0f;
    height = 300.0f;

    stage_body.setSize(sf::Vector2f(width, height));
    stage_body.setFillColor(sf::Color::White);
    stage_body.setPosition(x, y);
    
    panic_meter = 0.0f;

    panic_bar_bg.setSize(sf::Vector2f(200.f, 20.f));
    panic_bar_bg.setFillColor(sf::Color(50, 50, 50));
    panic_bar_bg.setPosition(1040.f, 40.f);

    panic_bar_fg.setFillColor(sf::Color(255, 0, 0)); 
    panic_bar_fg.setPosition(1040.f, 40.f);


} 



void Stage::init_camera(float window_w, float window_h) 
{
    camera.setSize(window_w, window_h); 
    camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    current_camera_pos = sf::Vector2f(0.f, 0.f);
}



void Stage::update_camera(float dt, const sf::Vector2f& player_pos, int player_dir_x, float panic_level) 
{
    current_camera_pos.x = player_pos.x;
    current_camera_pos.y = player_pos.y - 30.0f;


    float half_view_w = camera.getSize().x / 2.f; 
    float half_view_h = camera.getSize().y / 2.f;


    if (current_camera_pos.x < half_view_w) current_camera_pos.x = half_view_w;
    if (current_camera_pos.y < half_view_h) current_camera_pos.y = half_view_h;

    sf::Vector2f final_camera_pos = current_camera_pos;

    if (panic_level > 0.0f) 
    {
        float panic_ratio = panic_level / 100.0f;
        if (panic_ratio > 1.0f) panic_ratio = 1.0f;

        float current_intensity = (panic_ratio * panic_ratio) * 6.0f;

        float offset_x = (rand() % 100 - 50) / 50.0f * current_intensity;
        float offset_y = (rand() % 100 - 50) / 50.0f * current_intensity;

        final_camera_pos.x += offset_x;
        final_camera_pos.y += offset_y;
    }

    camera.setCenter(final_camera_pos);
}

bool Stage::load_from_tiled(const std::string& tmx_file_path)
{
    tmx::Map map;


    if (!map.load(tmx_file_path))
    {
        std::cerr << "Error: Could not open Tiled file at: " << tmx_file_path << std::endl;
        return false;
    }

    const auto& map_tilesets = map.getTilesets();

    if (!map_tilesets.empty()) 
    {
        std::string actual_image_path = map_tilesets[0].getImagePath(); 
        std::cout << "Found map image asset path from Tiled: " << actual_image_path << std::endl;
        
        if (!tileset_texture.loadFromFile(actual_image_path)) 
        {
            std::cerr << "Error: Could not load the required map image: " << actual_image_path << std::endl;
            return false; 
        }
    } 
    else 
    {
        std::cerr << "Error: No tilesets found inside your Tiled map file!" << std::endl;
        return false;
    }
    
    tileset_texture.setSmooth(false); 

    level_colliders.clear();
    map_triggers.clear();
    visual_layers.clear();

    auto map_size = map.getTileCount();
    auto tile_size = map.getTileSize();



    if (tile_size.x == 0 || tile_size.y == 0 || tileset_texture.getSize().x == 0) {
        std::cerr << "Error: Invalid map or tile dimension calculations." << std::endl;
        return false;
    }


    unsigned int texture_columns = tileset_texture.getSize().x / tile_size.x;

    const auto& layers = map.getLayers();
    for (const auto& layer : layers) 
    {
        if (layer->getType() == tmx::Layer::Type::Object) 
        {
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            const auto& objects = objectLayer.getObjects();
            
            for (const auto& object : objects) {
                tmx::FloatRect aabb = object.getAABB();
                sf::FloatRect scaled_hitbox(
                    aabb.left * 4.0f, 
                    aabb.top * 4.0f, 
                    aabb.width * 4.0f, 
                    aabb.height * 4.0f
                );

                bool is_sensor_trigger = false;
                std::string extracted_text = "";
                
                const auto& properties = object.getProperties();
                for (const auto& prop : properties) {
                    if (prop.getName() == "text") {
                        extracted_text = prop.getStringValue();
                        is_sensor_trigger = true;
                    }
                }

                if (is_sensor_trigger) {
                    DialogueTrigger dt;
                    dt.bounds = scaled_hitbox;
                    dt.text_content = extracted_text;
                    map_triggers.push_back(dt);
                } 
                else
                {
                    level_colliders.push_back(scaled_hitbox);
                }
            }
        }
        else if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();

            sf::VertexArray va(sf::Quads, map_size.x * map_size.y * 4);

            for (unsigned int tile_y = 0; tile_y < map_size.y; ++tile_y) {
                for (unsigned int tile_x = 0; tile_x < map_size.x; ++tile_x) {
                    unsigned int idx = tile_x + tile_y * map_size.x;
                    unsigned int gid = tiles[idx].ID;

                    if (gid == 0) continue; 

                    float screen_x = tile_x * tile_size.x * 4.0f;
                    float screen_y = tile_y * tile_size.y * 4.0f;

                    unsigned int tile_index = gid - 1;

                    // 2. THE GRID ALIGNMENT MATRICES FIX (Calculates clean X and Y wrapping positions)
                    float tex_x = (tile_index % texture_columns) * tile_size.x;
                    float tex_y = (tile_index / texture_columns) * tile_size.y; 

                    unsigned int v_idx = idx * 4;
                    
                    va[v_idx + 0].position = sf::Vector2f(screen_x, screen_y);
                    va[v_idx + 0].texCoords = sf::Vector2f(tex_x, tex_y);
                    
                    va[v_idx + 1].position = sf::Vector2f(screen_x + (tile_size.x * 4.0f), screen_y);
                    va[v_idx + 1].texCoords = sf::Vector2f(tex_x + tile_size.x, tex_y);
                    
                    va[v_idx + 2].position = sf::Vector2f(screen_x + (tile_size.x * 4.0f), screen_y + (tile_size.y * 4.0f));
                    va[v_idx + 2].texCoords = sf::Vector2f(tex_x + tile_size.x, tex_y + tile_size.y);
                    
                    va[v_idx + 3].position = sf::Vector2f(screen_x, screen_y + (tile_size.y * 4.0f));
                    va[v_idx + 3].texCoords = sf::Vector2f(tex_x, tex_y + tile_size.y);
                }
            }
            visual_layers.push_back(va);
        }
    }
    return true;
}



sf::FloatRect Stage::get_bounds() 
{
    return stage_body.getGlobalBounds(); 
}



sf::FloatRect Stage::create_platform(sf::RenderWindow& window, sf::Color color, float x, float y, float w, float h)
{
    sf::RectangleShape platform;
    platform.setPosition(sf::Vector2f(x, y));
    platform.setFillColor(color);
    platform.setSize(sf::Vector2f(w, h));
    window.draw(platform);

    return platform.getGlobalBounds();
}



void Stage::add_animated_prop(const std::string& id, const std::string& png_path, sf::Vector2f pos, sf::Vector2i frame_dim, unsigned int frame_count, float speed)
{
    AnimatedProp prop;
    if (prop.texture.loadFromFile(png_path)) {
        prop.texture.setSmooth(false);
        prop.sprite.setTexture(prop.texture);
        prop.sprite.setTextureRect(sf::IntRect(0, 0, frame_dim.x, frame_dim.y));
        prop.sprite.setScale(4.0f, 4.0f); // Upscale 4x to match Tiled map scaling rules
        prop.sprite.setPosition(pos);
        
        prop.id = id;
        prop.frame_size = frame_dim;
        prop.total_frames = frame_count;
        prop.current_frame = 0;
        prop.anim_timer = 0.0f;
        prop.anim_speed = speed;
        prop.is_playing = false;

        active_props.push_back(prop);
    }
}


void Stage::draw_stage(sf::RenderWindow& window)
{
    for (const auto& layer : visual_layers) {
        window.draw(layer, &tileset_texture);
    }
    
    // 👇 DRAW ANIMATED PROPS HERE 👇
    for (auto& prop : active_props) {
        window.draw(prop.sprite);
    }
    
    window.draw(subtext_prompt);
    window.draw(panic_bar_bg);
    window.draw(panic_bar_fg);
}

