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
	
	sf::Sprite sprite;


public:

	sf::RectangleShape collision_box;

	sf::Texture plr_texture;

	int directionX;
	int directionY;

	int jump_count;
	int max_jumps;

	int max_dash;
	int dash_count;
	int dash_timer;

	float x;
	float y;

	float target_velocity_x;

	float acceleration;

	bool is_moving;
	
	float dash_vel;
	
	float centerX;
	float centerY; 

	float speed;
	float jump_force;

	float width;
	float height; 

	bool is_grounded;
	bool can_dash;
	bool is_dashing;


	


	sf::Vector2f velocity;
	
	
	Direction direction;

	Player();

	sf::FloatRect get_bounds();

	void move(float dt);
	void jump();
	void update_pos(float dt);
	void draw_plr(sf::RenderWindow& window); 
	void draw_plr_sprite(sf::RenderWindow& window);



	void update_direction(Direction &currentDirection);

    void setTexture(const sf::Texture* texture);
    void setTextureRect(const sf::IntRect& rect);

	sf::Vector2f get_pos();



	void dash(sf::Vector2f &Velocity);




};

extern Player plr;