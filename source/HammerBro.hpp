/*
 * hammerbro.hpp
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

#ifndef HAMMERBRO_HPP
#define HAMMERBRO_HPP

/**
 * An enemy that jumps and throws hammers at the player.
 */
class HammerBro : public Enemy
{
public:
	static constexpr double WIDTH = 0.75; /**< The width of all hammer bros. */
	static constexpr double HEIGHT = 1.0; /**< The height of all hammer bros. */
	static constexpr double MOVEMENT_SPEED = 1.5; /**< The speed that all hammer bros move at. */
	static constexpr double JUMP_VELOCITY = 12.0; /**< The velocity of hammer bro jumps. */
	static constexpr double GRAVITY = -16.0; /**< The gravity experienced by hammer bros. */

	static constexpr int JUMP_WAIT_TIME = 300; /**< The time in frames that a hammer bro waits between jumps. */
	static constexpr int MOVE_DURATION = 60; /**< The time in frames that a hammer bro spends moving in one direction before turning around. */

	HammerBro();

private:
	int jumpTimer;
	int moveTimer;

	void onPreUpdate();
};
