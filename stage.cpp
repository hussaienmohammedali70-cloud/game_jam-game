#include <iostream>
#include <SFML/Graphics.hpp>

#include "stage.h"
#include "physics.h"
#include "player.h"

Player plr;
Physics physics;

Stage::Stage()
{
	x = 100.0f;
	y = 900.0f;

	width = 1700.0f;
	height = 300.0f;

	stage_body.setSize(sf::Vector2f(width, height));
	stage_body.setFillColor(sf::Color::White);
	stage_body.setPosition(x, y);
}


sf::FloatRect Stage::get_bounds() // get the stage box bounds //
{
	return stage_body.getGlobalBounds();
}

sf::FloatRect Stage::create_platform(sf::RenderWindow& window,sf::Color color, float x, float y, float  w, float h)
{
	sf::RectangleShape platform;

	platform.setPosition(sf::Vector2f(x, y));
	platform.setFillColor(color);
	platform.setSize(sf::Vector2f(w, h));
	window.draw(platform);

	sf::FloatRect platform_bounds = platform.getGlobalBounds();

	return platform_bounds;

}

void Stage::draw_stage(sf::RenderWindow& window)
{
	window.draw(stage_body);
}