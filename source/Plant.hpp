/*
 * Plant.hpp
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
#ifndef PLANT_HPP
#define PLANT_HPP

/**
 * Plants that come out of pipes.
 */
class Plant : public Enemy
{
public:
	/**
	 * Create a new plant.
	 *
	 * @param orientation the direction of the pipe that the plant will be in.
	 */
	Plant( Direction orientation = UP );

	static constexpr int PHASE_DURATION = 30; /**< The time that the plant spends in each phase. */
	static constexpr double WAIT_DISTANCE = 4.0; /**< The distance that the player needs to be within for a plant to wait in a pipe. */

private:
	/**
	 * The phases that the plant goes through.
	 */
	enum Phase
	{
		PHASE_WAIT,
		PHASE_EXIT,
		PHASE_ATTACK,
		PHASE_ENTER
	};

	Direction orientation;
	Phase phase;
	int phaseTimer;

	void onPreUpdate();
};

#endif // PLANT_HPP
