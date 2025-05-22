/*
 * growingladder.cpp
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

#include "GrowingLadder.hpp"
#include "Ladder.hpp"
#include "Tile.hpp"
#include "World.hpp"

GrowingLadder::GrowingLadder()
{
	setGravityEnabled(false);
	setSpriteCollisionsEnabled(false);
	setAnimation("growing_ladder");
	setYVelocity(GROWING_LADDER_SPEED);
}

void GrowingLadder::grow( double yPosition )
{
	// Get the tile at the bottom of us
	int x = static_cast<int>(std::floor(getCenterX()));
	int y = static_cast<int>(std::floor(yPosition));

	if( y >= getWorld().getHeight() )
	{
		kill();
		return;
	}

	const Tile* tile = getWorld().getTile(x, y);

	if( tile == nullptr )
	{
		getWorld().setTile(x, y, new Ladder());
	}
}

void GrowingLadder::onCollision( Tile& tile, Edge edge )
{
	// If we collide with a tile, it's time to stop on the next frame...
	if( tile.getCollisionBehavior() != TILE_NONE )
	{
		grow(getCenterY());
		kill();
	}
}

void GrowingLadder::onPreUpdate()
{
	grow(getBottom());
}
