/*
 * item.hpp
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
#ifndef ITEM_HPP
#define ITEM_HPP

/**
 * An item/power-up usable by the Player.
 */
class Item : public Sprite
{
public:
	static constexpr double WIDTH = 0.75;  /**< The width of all items. */
	static constexpr double HEIGHT = 0.75; /**< The height of all items. */

	static constexpr int POWERUP_POINT_VALUE = 1000;  /**< The default point value for powerups. */

	/**
	 * Get the point value of the Item when it is collected.
	 */
	int getPointValue() const;

protected:
	Item( int pointValue = POWERUP_POINT_VALUE );

	virtual void onCollision(Tile& tile, Edge edge);

private:
	int pointValue;
};

