#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Dialog 
{
private:
    sf::RectangleShape dialog_box;
    sf::Font font;
    sf::Text text_display;
    
    float x, y, width, height;
    bool is_visible;

    std::vector<std::string> dialogue_lines;
    unsigned int current_index;

    // 👇 FIXED: Explicitly declared here so dialog.cpp can use them!
    std::string full_text;       
    unsigned int current_char;   
    float type_timer;            

public:
    Dialog();
    bool init_dialog(const std::string& font_path);
    
    void start_dialogue(const std::vector<std::string>& lines);
    void advance(); 
    
    void update(float dt); 

    void show() { is_visible = true; }
    void hide() { is_visible = false; }
    bool active() const { return is_visible; }
    void draw_dialog(sf::RenderWindow& window);
};
