/*
 * enums.hpp
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



#include <string>
#include "Vector2.hpp"

#ifndef ENUMS_HPP
#define ENUMS_HPP

/**
 * A cardinal direction.
 */
enum Direction
{
	DOWN,  /**< Down. */
	LEFT,  /**< Left. */
	RIGHT, /**< Right. */
	UP     /**< Up. */
};

/// @todo repurpose this in certain use cases
enum Edge
{
	EDGE_BOTTOM,
	EDGE_LEFT,
	EDGE_RIGHT,
	EDGE_TOP
};

/**
 * Get a direction constant from a string value.
 */
Direction getDirection( const std::string& name );

Direction getOppositeDirection( Direction direction );
Edge getOppositeEdge( Edge edge );

/**
 * Get a unit vector in the direction specified.
 */
Vector2<int> getVector( Direction direction );

/**
 * Check if a Direction is horizontal (LEFT or RIGHT).
 */
bool isHorizontal( Direction direction );

/**
 * Check if a Direction is vertical (UP or DOWN).
 */
bool isVertical( Direction direction );
