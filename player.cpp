#include <iostream>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

#include "player.h"





Player::Player() // player constructor //
{
	direction.x= 0;
	direction.y = 1;

	jump_count = 0;
	max_jumps = 3; 

	max_dash = 2;
	dash_count = 0;

	dash_vel = 3000.0f; 

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



void Player::dash(sf::Vector2f &Velocity)
{

	if(direction.x == 0 && direction.y == 0) return;
	
	if(dash_count < max_dash)
	{

		float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));


		Velocity.x = dash_vel * (direction.x / length);
		velocity.y = dash_vel * (direction.y / length);
		
		dash_count += 1;
	}

	if(is_grounded)
	{
		dash_count = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////

void Player::move(float dt) // movement function //
{
	target_velocity_x = 0.0f; // resets the x velocity so the player doesnt slide endlessly //

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		target_velocity_x = -speed;
		direction.x = -1;
	}

	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		target_velocity_x = speed;
		direction.x = 1; 
		
	}
	else
	{
		target_velocity_x = 0.0f;
		direction.x = 0;
	}


	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		direction.y = -1;
	}

	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		direction.y = 1;
	}

	else
	{
		direction.y = 0;
	}



	float slide_friction = 9.0f; //slide friction obviously


	float lerp_factor = slide_friction * dt; // smoothes out the movement // 

	if(lerp_factor > 1.0f) lerp_factor = 1.0f;

	velocity.x = velocity.x + (target_velocity_x - velocity.x ) * lerp_factor;

}

//////////////////////////////////////////////////////////////////////////////////////////////////




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


void Player::update_direction(Direction &currentDirection)
{
	currentDirection.x = direction.x;
	currentDirection.y = direction.y;
}


void Player::draw_plr(sf::RenderWindow& window) // draw the sprite to the window //
{
	window.draw(sprite);
}



