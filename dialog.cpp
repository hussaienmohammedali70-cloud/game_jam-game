#include "dialog.h"
#include <iostream>

Dialog::Dialog() 
{
    width = 1200.0f;
    height = 200.0f;
    x = 40 + (1920.0f - width) / 2.0f; 
    y = 800.0f;

    dialog_box.setSize(sf::Vector2f(width, height));
    dialog_box.setFillColor(sf::Color(0, 0, 0, 220)); 
    dialog_box.setOutlineColor(sf::Color(150, 150, 150)); 
    dialog_box.setOutlineThickness(3.0f);
    dialog_box.setPosition(x, y);

    is_visible = false; 
    current_index = 0;
    
    // Initialize typewriter variables // 
    full_text = "";
    current_char = 0;
    type_timer = 0.0f;
}

bool Dialog::init_dialog(const std::string& font_path)
{
    if (!font.loadFromFile(font_path)) 
    {
        std::cerr << "Error: Font loading failed!" << std::endl;
        return false;
    }


    text_display.setFont(font);
    text_display.setCharacterSize(24);
    text_display.setFillColor(sf::Color::White);
    text_display.setPosition(x + 40.0f, y + 40.0f);
    return true;
}

void Dialog::start_dialogue(const std::vector<std::string> &lines)
{
    dialogue_lines = lines;
    current_index = 0;
    
    if (!dialogue_lines.empty()) 
    {
        full_text = dialogue_lines[current_index];
        current_char = 0;
        type_timer = 0.0f;
        text_display.setString(""); // Clear old display text
        show(); 
    }
}

void Dialog::advance()
{
    if (!is_visible) return;

    //skipping lol// 
    if (current_char < full_text.length()) {
        current_char = full_text.length();
        text_display.setString(full_text);
        return;
    }

    current_index++;

    if (current_index >= dialogue_lines.size()) 
    {
        hide();
        return; 
    } 

    // Reset typewriter offsets for the brand new page sequence
    full_text = dialogue_lines[current_index];
    current_char = 0;
    type_timer = 0.0f;
    text_display.setString("");
}

void Dialog::update(float dt)
{
    if (!is_visible) return;

    
    if (current_char < full_text.length())
     {
        type_timer += dt;
        
        float typing_speed = 0.02f; 
        
        while (type_timer >= typing_speed && current_char < full_text.length()) {
            type_timer -= typing_speed;
            current_char++;
            
            // Slice out a substring from character 0 to our current typing step index
            text_display.setString(full_text.substr(0, current_char));
        }
    }
}

void Dialog::draw_dialog(sf::RenderWindow& window)
{
    if (!is_visible) return;
    window.draw(dialog_box);
    window.draw(text_display);
}
