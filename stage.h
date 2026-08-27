#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>


class Stage
{
private:
	sf::RectangleShape stage_body;

public:

	float width;
	float height;

	float x;
	float y;

	Stage();

	void draw_stage(sf::RenderWindow& window);

	void setup_stage();
	
	sf::FloatRect get_bounds();

	sf::FloatRect create_platform(sf::RenderWindow& window, sf::Color color ,float x, float y, float w, float h); //x y width and height 
	

};