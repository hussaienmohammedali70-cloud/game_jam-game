#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <array>

#include "player.h"
#include "physics.h"
#include "stage.h"

int main()
{
	Player plr;
	Physics physics; // class instances //
	Stage stage;

	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "stickBANG");

	while(window.isOpen()) //main game loop 
	{
		float dt = clock.restart().asSeconds();
		sf::Event event;



		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}



			if(event.type == sf::Event::KeyPressed) // jump logic //
			{
				if(event.key.code == sf::Keyboard::Space)
				{
					plr.jump();
				}

				if(event.key.code == sf::Keyboard::L)
				{
					plr.dash(plr.velocity);
				}

			}

			std::cout << plr.velocity.y << '\n';
		
		}

		if(plr.is_grounded)
		{
			plr.dash_count = 0;
		}
		plr.move(dt); // plr movement junk //


		physics.apply_gravity(plr, dt); // physics functions //


		plr.update_pos(dt); // updates the position of the player // 
		physics.collide(plr, stage.get_bounds()); //collide with the stage 

		




		//NOTE TO FUTURE ME: USE THE COLLISION FUNCTION FOR FUCKING EVERYTHING // 


		window.clear(sf::Color::Black); // window initilization //
		stage.draw_stage(window);
		physics.collide(plr, stage.create_platform(window, sf::Color::White, 300.0f, 400.0f, 300.0f, 50.0f));
		plr.draw_plr(window); //draw the player to the screen
		

		window.display(); 




		
	}	// main game loop end // 
	return 0;
}