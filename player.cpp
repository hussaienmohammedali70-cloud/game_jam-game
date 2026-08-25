#include <iostream>
#include <SFML/Graphics.hpp>

#include "player.h"
#include "physics.h"



Physics physics;


Player::Player() // player constructor //
{
	direction = 1; 

	jump_count = 0;
	max_jumps = 3; 

	max_dash = 2;
	dash_count = 0;

	x = 100.0f; //player position
	y = 100.0f;

	velocity = sf::Vector2f(0.0f, 0.0f); //player velocity


	acceleration = 12.0f;
		
	speed = 900.0f; // speed duh //

	jump_force = 1900.0f;

	width = 40.0f;
	height = 100.0f;

	is_grounded = true;
	can_dash = true;

	sprite.setSize(sf::Vector2f(width, height)); // sprite properties //
	sprite.setPosition(x, y);
	sprite.setFillColor(sf::Color::White); 
}




sf::FloatRect Player::get_bounds() // return the bounds of the hitbox // 
{
	return sprite.getGlobalBounds();
}


void Player::jump()
{
	if(is_grounded)
	{
		jump_count = 0;
	}

	if(jump_count < max_jumps)
	{
		velocity.y = -jump_force; // self explanitory //
		jump_count += 1;

		is_grounded = false;
	}


}



void Player::dash(sf::Vector2f velocity)
{
	if(is_grounded)
	{
		dash_count = 0;
	}

	if(dash_count < max_dash)
	{
		velocity.x 
	}
}

void Player::move(float dt) // movement function //
{
	target_velocity_x = 0.0f; // resets the x velocity so the player doesnt slide endlessly //

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		target_velocity_x = -speed;
		direction *= -1;
	}

	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		target_velocity_x = speed;
		direction *= -1; 
		
	}
	else
	{
		target_velocity_x = 0.0f;
	}

	float slide_friction = 9.0f; //slide friction obviously


	float lerp_factor = slide_friction * dt; // smoothes out the movement // 

	if(lerp_factor > 1.0f) lerp_factor = 1.0f;

	velocity.x = velocity.x + (target_velocity_x - velocity.x ) * lerp_factor;

}



void Player::update_sprite_only()
{
	sprite.setPosition(x, y);
}

void Player::update_pos(float dt) // bind the velocity to the position of the player //
{
	x += velocity.x * dt;
	y += velocity.y * dt;

	sprite.setPosition(x, y);
}



void Player::draw_plr(sf::RenderWindow& window) // draw the sprite to the window //
{
	window.draw(sprite);
}



