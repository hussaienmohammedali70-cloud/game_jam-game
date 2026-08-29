#include <SFML/Graphics.hpp>


#include "animation.h"
#include "player.h"


Direction direction;

Animation::Animation(sf::Texture* texture, sf::Vector2u image_count) {
    this->image_count = image_count;
    total_time = 0.0f;
    current_image.x = 0;
    current_image.y = 0; 

    // Use integer division directly
    texture_rect.width = texture->getSize().x / image_count.x;
    texture_rect.height = texture->getSize().y / image_count.y;
}


void Animation::update(int row, int frames, float switch_time, float dt) {
    if (current_image.y != row) {
        current_image.y = row;
        current_image.x = 0;
    }

    total_time += dt;
    if(total_time >= switch_time) 
    {
        total_time -= switch_time;

        current_image.x++;
        if(current_image.x >= frames) 
        {
            current_image.x = 0;
        }
    }

    // 1. Calculate standard positioning
    texture_rect.top = current_image.y * texture_rect.height;

    if (plr.direction.x > 0)
     {

        texture_rect.width = abs(texture_rect.width); 
        texture_rect.left = current_image.x * texture_rect.width;
    } 
    else if (plr.direction.x < 0) {
        // Moving Left
        texture_rect.width = -abs(texture_rect.width);
        texture_rect.left = (current_image.x + 1) * abs(texture_rect.width);
    }
    else 
    {
        if (texture_rect.width > 0) {
            // Was facing right, stay facing right
            texture_rect.left = current_image.x * texture_rect.width;
        } 
        else 
        {
            texture_rect.left = (current_image.x + 1) * abs(texture_rect.width);
        }
    }
}
