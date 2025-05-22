/*
 * koopa.cpp
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

#ifndef KOOPA_HPP
#define KOOPA_HPP

/**
 * A turtle-like enemy with a kickable shell.
 */
class Koopa : public Enemy
{
public:
	static constexpr double WIDTH = 0.75;  /**< The width of all koopas. */
	static constexpr double HEIGHT = 0.75; /**< The height of all koopas. */

	static constexpr double MOVEMENT_SPEED = 1.5; /**< The speed that koopas move at. */

	Koopa();

private:
	void onBump();
	void onStomp();
};

