/*
 * Paratroopa.hpp
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

#include "Enemy.hpp"
#ifndef PARATROOPA_HPP
#define PARATROOPA_HPP

/**
 * A flying koopa.
 */
class Paratroopa : public Enemy
{
public:
	static constexpr double WIDTH = 0.75;  /**< The width of all paratroopas. */
	static constexpr double HEIGHT = 0.8; /**< The height of all paratroopas. */

	static constexpr double MOVEMENT_SPEED = 1.5; /**< The speed that paratroopas move at. */

	static constexpr double BOUNCE_SPEED = 8.0; /**< The speed that paratroopas bounce when they hit the ground. */

	static constexpr double GRAVITY = -16.0; /**< The gravity of a paratroopa. */

	Paratroopa();

private:
	void onBump();
	void onPostUpdate();
	void onStomp();
};
