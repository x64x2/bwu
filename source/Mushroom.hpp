/*
 * Mushroom.hpp
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

#include "Item.hpp"

/**
 * Types of mushroom items.
 */
enum MushroomType
{
	MUSHROOM_SUPER, /**< Makes the player super. */
	MUSHROOM_1UP    /**< Grants the player an extra life. */
};

/**
 * A mushroom item.
 */
class Mushroom : public Item
{
public:
	static constexpr double MOVEMENT_SPEED = 3.0; /**< The movement speed of all mushrooms. */

	/**
	 * Create a new mushroom.
	 */
	Mushroom( MushroomType type = MUSHROOM_SUPER );

	/**
	 * Get the type of mushroom.
	 */
	MushroomType getType() const;

private:
	MushroomType type;
};
