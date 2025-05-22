/*
 * game.hpp
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
#include <list>
#ifndef GAME_HPP
#define GAME_HPP

class GameState;

/**
 * A class that manages GameStates and execution flow.
 */
class Game
{
public:
	/**
	 * Create the Game with an initial state.
	 *
	 * @param initialState the initial GameState to execute.
	 */
	Game( GameState* initialState );

	~Game();

	/**
	 * Get the number of frames that have been executed since the program started.
	 */
	int getFrameCount() const;

	/**
	 * Check if the Game is still running.
	 *
	 * @return whether there is a GameState on the stack.
	 */
	bool isRunning() const;

	/**
	 * Pop the current GameState off the stack.
	 */
	void popState();

	/**
	 * Push a GameState onto the stack. The current GameState remains paused
	 * until this new state is popped.
	 *
	 * @param state the GameState to push.
	 */
	void pushState( GameState* state );

	/**
	 * Pop all states off the stack and end execution.
	 */
	void quit();

	/**
	 * Switch the GameState to a different state. This is equivalent to popping
	 * and then pushing the new state to the stack.
	 *
	 * @param state the GameState to switch to.
	 */
	void switchState( GameState* state );

	/**
	 * Update the Game (run one iteration of the loop in the current state).
	 */
	void update();

private:
	std::list<GameState*> deadStateList;
};

