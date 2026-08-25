#include <SFML/Graphics.hpp>

#include "enemy.h"


Enemy::Enemy()
{
    x = 400;
    y = 100;

    width = 40;
    height = 60;

    is_grounded = false; 
}