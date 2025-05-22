/*
 * growingladder.hpp
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


#include "Sprite.hpp"

#ifndef GROWINGLADDER_HPP
#define GROWINGLADDER_HPP

/**
 * A sprite that grows a ladder upward until it hits a block or the bounds
 * of the world.
 */
class GrowingLadder : public Sprite
{
public:
	static constexpr double GROWING_LADDER_SPEED = 4.0;

	GrowingLadder();

private:
	void grow( double yPosition );
	void onCollision( Tile& tile, Edge edge );
	void onPreUpdate();
};

