#pragma once

#include <SFML/Graphics.hpp>


class Enemy
{
private:
    sf::RectangleShape EnemyBody;

public:
    
    float x;
    float y;
    
    float width;
    float height;


    bool is_grounded; 

    sf::Vector2f velocity;
    Enemy();

};