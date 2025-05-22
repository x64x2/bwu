
/*
 * lakitu.cpp
 * 
 * Copyright 2025 x64x2 <x64x2@mango>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "Lakitu.hpp"
#include "Particle.hpp"
#include "Player.hpp"
#include "Random.hpp"
#include "Spiny.hpp"
#include "World.hpp"

/**
 * An egg that spawns a spiny upon impacting the ground.
 */
class SpinyEgg : public Enemy
{
public:
	static constexpr double GRAVITY = -32.0;

	SpinyEgg()
	{
		setAnimation("spiny_egg");
		setFireproof(true);
		setStompable(false);
		setGravityEnabled(false);
		setYAcceleration(GRAVITY);
	}

private:
	void onGround()
	{
		Player* player = getWorld().getPlayer();
		Spiny* spiny = new Spiny;
		if( player != nullptr && player->getCenterX() > getCenterX() )
		{
			spiny->setXVelocity( -1 * spiny->getXVelocity() );
		}
		spiny->setBottom( std::ceil(getBottom()) + 0.1 );
		spiny->setCenterX( getCenterX() );
		getWorld().addSprite(spiny);
		kill();
	}
};

Lakitu::Lakitu() :
	throwTimer(0)
{
	setBounceOffEnemiesEnabled(false);
	setGravityEnabled(false);
	setAnimation("lakitu");
}

void Lakitu::onPreUpdate()
{
	// Get the player that we are following
	Player* player = getWorld().getPlayer();
	if( player == nullptr )
	{
		return;
	}

	// Update movement
	int sign = 1;
	if( getCenterX() > player->getCenterX() )
	{
		sign = -1;
	}
	setXAcceleration( sign * 16.0 );
	setMaximumXSpeed( player->getMaximumXSpeed() + 2.0 );

	// Are we throwing?
	if( throwTimer > 0 )
	{
		throwTimer--;
		if( throwTimer == 0 )
		{
			// Throw a spiny
			SpinyEgg* egg = new SpinyEgg;
			egg->setCenterX( getCenterX() );
			egg->setCenterY( getCenterY() );
			egg->setXVelocity((player->getCenterX() - getCenterX()));
			egg->setYVelocity( 12.0 );
			getWorld().addSprite( egg );

			setAnimation("lakitu");
		}
	}
	else
	{
		// See if we should throw another spiny now, randomly
		if( getWorld().getRandom().nextInt() % 120 == 0 )
		{
			throwTimer = THROW_DURATION;
			playAnimation("lakitu_ducking", "lakitu_throwing");
		}
	}
}
