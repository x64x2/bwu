/*
 * LevelEnd.cpp
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
#include "Globals.hpp"
#include "LevelEnd.hpp"
#include "Particle.hpp"
#include "Random.hpp"
#include "World.hpp"

LevelEnd::LevelEnd()
{
	setAnimation("level_end_block");
	setGravityEnabled(false);
}

void LevelEnd::onPreUpdate()
{
	if( getWorld().getFrameNumber() % 4 != 0 )
	{
		return;
	}

	Random& random = getWorld().getRandom();
	Particle* p = new Particle( getAnimation("coin_sparkle"), true );
	p->disableCollisions();
	p->disableGravity();
	p->setLayer(2);
	p->setCenterX(getLeft() + random.nextReal() * getWidth());
	p->setCenterY(getBottom() + random.nextReal() * getHeight());
	p->setXVelocity((random.nextReal() * 2.0 - 1.0) * 3.0);
	p->setYVelocity((random.nextReal() * 2.0 - 1.0) * 3.0);
	getWorld().addSprite( p );
}
