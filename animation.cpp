#include <SFML/Graphics.hpp>


#include "animation.h"


Animation::Animation(sf::Texture* texture, sf::Vector2u image_count, float switch_time)
{
    this->image_count = image_count;
    this->switch_time = switch_time;

    total_time = 0.0f;

    current_image.x = 0;

    texture_rect.width = texture->getSize().x / float(image_count.x);
    texture_rect.height = texture->getSize().y / float(image_count.y);
}

    void Animation::update(int row, float dt)
    {
        current_image.y = row;
        total_time += dt;

        if(total_time > switch_time)
        {
            total_time -= switch_time;
            current_image.x++;

            if(current_image.x >= image_count.x)
            {
                current_image.x = 0;
         
         
         
            }
        }

        texture_rect.left = current_image.x * texture_rect.width;
        texture_rect.top = current_image.y * texture_rect.height;


    }

    
    
