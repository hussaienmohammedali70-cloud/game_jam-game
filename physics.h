#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "player.h"


class Physics
{
public:

	float gravity;

	Physics();

	void check_collision();
	
	void collide(Player& player, sf::FloatRect bounds);

	void apply_gravity(Player& player, float dt);


	bool affected_by_gravity; 



};


extern Physics physics;