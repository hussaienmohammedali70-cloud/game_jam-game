#include <iostream>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

#include "player.h"
#include "physics.h"
#include "stage.h"

Stage stage;





Physics physics;


Player::Player() // player constructor //
{

	x = 600.0f; //player position
	y = 500.0f;


	width = 40.0f;
	height = 60.0f;

	collision_box.setSize(sf::Vector2f(width, height));

	collision_box.setFillColor(sf::Color::Transparent);

	collision_box.setOutlineColor(sf::Color::Green);

	collision_box.setOutlineThickness(1.0f);

	collision_box.setPosition(x, y);

	
	plr_texture.loadFromFile("player_spriteSheet.png");
	plr_texture.setSmooth(false);

	sprite.setTexture(plr_texture);
	
	

	sprite.setScale(6.0f, 6.0f);
	
	direction.x= 0;
	direction.y = 1;

	jump_count = 0;
	max_jumps = 2; 

	is_moving = false;

	max_dash = 1;
	dash_count = 0;



	dash_vel = 2600.0f;
	
	dash_timer = 0.5f;

	


	velocity = sf::Vector2f(0.0f, 0.0f); //player velocity


	acceleration = 12.0f;
		
	speed = 500.0f; // speed duh //

	jump_force = 1600.0f;


	is_grounded = true;

	can_dash = true;
	is_dashing = false;






	sf::FloatRect sprite_bounds = sprite.getGlobalBounds();

	float offset_x = (width - sprite_bounds.width) / 2.0f;
	float offset_y = (height - sprite_bounds.height) / 2.0f;

	sprite.setPosition(x + offset_x, y + offset_y + 18.0f);
	
}



void Player::setTexture(const sf::Texture* texture) {

	if(texture != nullptr)
	{
		sprite.setTexture(*texture);

		sprite.setScale(5.0f, 6.0f);
	}
    
}


void Player::setTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);

	sprite.setScale(5.0f, 6.0f);

}






sf::FloatRect Player::get_bounds() // return the bounds of the hitbox // 
{
	return collision_box.getGlobalBounds();
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
		if(can_dash)
		{
			float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));


			Velocity.x = dash_vel * (direction.x / length);
			velocity.y = dash_vel * (direction.y / length);
		
			dash_count += 1;

		}

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
		velocity.x = 0;
		target_velocity_x = 0.0f;
		direction.x = 0; 


		
	}

	if(velocity.x != 0)
	{
		is_moving = true;
	}
	else
	{
		is_moving = false;
	}
	

	// directional input and other boring stuff //

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


	
	sf::FloatRect player_box = plr.get_bounds();




}

//////////////////////////////////////////////////////////////////////////////////////////////////


sf::Vector2f Player::get_pos()
{
	return sf::Vector2f(x, y);
}



void Player::update_pos(float dt) // bind the velocity to the position of the player //
{
	x += velocity.x * dt;
	y += velocity.y * dt;

	collision_box.setPosition(x, y); 
	sf::FloatRect sprite_bounds = sprite.getGlobalBounds(); // the bounds of the sprite not the collision box vro // 

	float offset_x = (width - sprite_bounds.width) / 2.0f;  // get the offset so the sprite doesnt spawn on the bottom left corner of the collision box // 
	float offset_y = (height - sprite_bounds.height) / 2.0f;

	sprite.setPosition(x + offset_x, y + offset_y + 18.0f); // set the position to be at the center of the collision box //
}





 
void Player::update_direction(Direction &currentDirection)
{
	currentDirection.x = direction.x;
	currentDirection.y = direction.y;
}



void Player::draw_plr(sf::RenderWindow& window) // draw the sprite to the window //
{
	window.draw(collision_box);
}




void Player::draw_plr_sprite(sf::RenderWindow& window)
{
	window.draw(sprite);
}



bool Player::swap_appearance(const std::string& new_png_path)
{
    
    if (!plr_texture.loadFromFile(new_png_path))
    {
        std::cerr << "Error: Mid-game texture swap failed! Could not find: " << new_png_path << std::endl;
        return false; 
    }

    
    plr_texture.setSmooth(false);

    
    sprite.setTexture(plr_texture);
    
    return true;
}


