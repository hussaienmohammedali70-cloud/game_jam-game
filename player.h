#pragma once

#include <SFML/Graphics.hpp>
#include <array>

struct Direction
{
		int x;
		int y;
};


class Player
{
private:
	sf::RectangleShape sprite;


public:

	int directionX;
	int directionY;

	int jump_count;
	int max_jumps;

	int max_dash;
	int dash_count;

	float x;
	float y;

	float target_velocity_x;

	float acceleration;
	
	float dash_vel;
	
	float centerX;
	float centerY; 

	float speed;
	float jump_force;

	float width;
	float height; 

	bool is_grounded;
	bool can_dash;


	sf::Vector2f velocity;
	
	
	Direction direction;

	Player();

	sf::FloatRect get_bounds();

	void move(float dt);
	void jump();
	void update_pos(float dt);
	void draw_plr(sf::RenderWindow& window); 
	void update_sprite_only();


	void update_direction(Direction &currentDirection);


	void dash(sf::Vector2f &Velocity);



};