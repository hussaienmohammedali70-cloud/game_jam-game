#include <iostream>
#include <SFML/Graphics.hpp>

#include "physics.h"
#include "player.h"
#include "stage.h"



Physics::Physics() // physics class constructor //
{
	gravity = 6500.0f;
}


bool check_collision(sf::FloatRect object1, sf::FloatRect object2)
{
	bool colliding = false;


	float object1_centerX = object1.left + (object1.width / 2.0);
	float object1_centerY = object1.top + (object1.height / 2.0); // player centers //

	float object2_centerX = object2.left + (object2.width / 2.0);
	float object2_centerY = object2.top + (object2.height / 2.0); // stage centers //


	float distanceX = object1_centerX - object2_centerX;
	float distanceY = object1_centerY - object2_centerY; // distances between the player and the stage //


	float min_safeX = (object1.width / 2.0) + (object2.width / 2.0); // minimumm safe distances //
	float min_safeY = (object1.height / 2.0) + (object2.height / 2.0); 


	float overlapX = min_safeX - std::abs(distanceX);
	float overlapY = min_safeY - std::abs(distanceY); // overlap //


	if(overlapX > 0.0f || overlapY > 0.0f)
	{
		colliding = true;
	}


	return colliding;
	

}




void Physics::collide(Player& plr, sf::FloatRect stage) //check if the player is colliding //
{
	float player_centerX = plr.get_bounds().left + (plr.width / 2.0);
	float player_centerY = plr.get_bounds().top + (plr.height / 2.0); // player centers //

	float stage_centerX = stage.left + (stage.width / 2.0);
	float stage_centerY = stage.top + (stage.height / 2.0); // stage centers //


	float distanceX = player_centerX - stage_centerX;
	float distanceY = player_centerY - stage_centerY; // distances between the player and the stage //


	float min_safeX = (plr.width / 2.0) + (stage.width / 2.0); // minimumm safe distances //
	float min_safeY = (plr.height / 2.0) + (stage.height / 2.0); 


	float overlapX = min_safeX - std::abs(distanceX);
	float overlapY = min_safeY - std::abs(distanceY); // overlap //


	if(overlapX > 0.0f && overlapY > 0.0f)
	{

		if(overlapY > overlapX)
		{
			if(distanceX > 0.0f)
			{
				
				plr.x += overlapX;
			}
			else
			{
				plr.x -= overlapX;
			}

		}
	    
	    else
	    {
	    	if(distanceY > 0.0f)
	    	{
	    		plr.y += overlapY;
	    	}
	    	else
	    	{
	    		plr.y -= overlapY;
	    		plr.is_grounded = true;
	    	}

	        plr.velocity.y = 0.0f;


	    
	    }

	}

	
}

void Physics::apply_gravity(Player& player, float dt)
{
	
	player.velocity.y += gravity * dt;
	
}




