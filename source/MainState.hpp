/*
 * MainState.hpp
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

#include "GameState.hpp"

#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

class DeadPlayer;
class Player;
class World;

/**
 * The main state of the Game.
 */
class MainState : public GameState
{
public:
	MainState(int level);
	~MainState();

	void update();

private:
	World* world;
	bool commandMode;
	bool paused;
	std::string commandString;

	Player* player;
	DeadPlayer* deadPlayer;
	bool playerDeathHandled;

	int endTimer; /**< Counts down until we pop the current game state. */
	int initialEndTimer; /**< The initial value of the end timer when it was set. */
	int remainingTime; /**< Used when calculating the player's bonus from level time. */

	void executeCommand();
	void input();
	void render();
};
