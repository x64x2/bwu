/*
 * goomba.hpp
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

#ifndef GOOMBA_HPP
#define GOOMBA_HPP

/**
 * A simple enemy.
 */
class Goomba : public Enemy
{
public:
	static constexpr double DEAD_PARTICLE_DURATION = 0.5; /**< The duration of the particle spawned when a goomba is stomped. */
	static constexpr double MOVEMENT_SPEED = 1.5;         /**< The movement speed of all goombas. */
	static constexpr double WIDTH = 0.75;                 /**< The width of all goombas. */
	static constexpr double HEIGHT = 0.75;                /**< The height of all goombas. */

	Goomba();

private:
	void onStomp();
};
