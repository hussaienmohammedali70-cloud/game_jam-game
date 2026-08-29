#include <SFML/Graphics.hpp>


class Animation
{
private:

    sf::Vector2u image_count; // unsigned integer that represents the number of the images idk kinda self explanitory 
    sf::Vector2u current_image; // current image in the animaiton //

    float total_time; // total animation time // 
    float switch_time; // the time it takes the animation to switch images // //switch my animation twin //

public:
    Animation(sf::Texture* texture, sf::Vector2u image_count);

    sf::IntRect texture_rect; // rectangle of the texture, again im explaining the most obvious things

    void update(int row, int frames, float switch_time, float dt);

};