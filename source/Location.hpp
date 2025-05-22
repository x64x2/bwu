/*
 * Location.hpp
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


#ifndef LOCATION_HPP
#define LOCATION_HPP

/**
 * An exact location within the World.
 */
struct Location
{
	int levelId; /**< The Level id number. */
	int x;       /**< The x coordinate within the Level. */
	int y;       /**< The y coordinate within the Level. */

	Location();
};

static constexpr int CURRENT_LEVEL = -1; /**< The ID used to reference the current Level. */
static constexpr int NULL_LEVEL = 0;     /**< The ID used to reference no Level. */

