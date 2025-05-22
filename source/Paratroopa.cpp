/*
 * Paratroopa.cpp
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


#include "Koopa.hpp"
#include "Paratroopa.hpp"
#include "World.hpp"

Paratroopa::Paratroopa()
{
	setDefaultXVelocity(-1.0 * MOVEMENT_SPEED);
	setWidth(WIDTH);
	setHeight(HEIGHT);
	setGravityEnabled(false);
	setYAcceleration(GRAVITY);

	setAnimation("paratroopa");
}

void Paratroopa::onBump()
{
	Koopa* k = new Koopa;
	k->setX(getX());
	k->setY(getY());
	k->setYVelocity(BUMP_VELOCITY);
	getWorld().addSprite(k);
	kill();
}

void Paratroopa::onPostUpdate()
{
	if( !isFlying() )
	{
		setYVelocity(BOUNCE_SPEED);
	}
}

void Paratroopa::onStomp()
{
	Koopa* k = new Koopa;
	k->setX(getX());
	k->setY(getY());

	getWorld().addSprite(k);

	kill();
}
