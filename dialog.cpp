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
    current_index = 0; // Initialize index tracker
}

bool Dialog::init_dialog(const std::string& font_path)
{
    if (!font.loadFromFile(font_path)) {
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
    
    if (!dialogue_lines.empty()) {
        text_display.setString(dialogue_lines[current_index]);
        show(); // Automatically turn visible when new dialogue starts
    }
}

void Dialog::advance()
{
    if (!is_visible) return;

    current_index++;

    if (current_index >= dialogue_lines.size()) 
    {
        hide();
        return; 
    } 

    text_display.setString(dialogue_lines[current_index]);
}


void Dialog::draw_dialog(sf::RenderWindow& window)
{
    if (!is_visible) return;
    window.draw(dialog_box);
    window.draw(text_display);
}
