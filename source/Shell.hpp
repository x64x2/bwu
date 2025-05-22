/*
 * Shell.hpp
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
#ifndef SHELL_HPP
#define SHELL_HPP

class Player;

/**
 * A Koopa shell.
 */
class Shell : public Enemy
{
public:
	static constexpr double WIDTH = 0.75;  /**< The width of all shells. */
	static constexpr double HEIGHT = 0.75; /**< The height of all shells. */

	static constexpr double MOVEMENT_SPEED = 12.0; /**< The speed that shells move at when kicked. */

	static constexpr int KICK_TIMER = 10; /**< Number of frames after a shell is kicked that it cannot be stomped. */

	Shell( bool fireproof = false );

	/**
	 * Get the player that last kicked the shell.
	 */
	Player* getPlayer();

private:
	int kickTimer; /**< Timer set after a shell is kicked to prevent stomping immediately until the shell has moved out of range. */
	int kills; /**< Number of kills accumulated since the shell was kicked by a player. */
	Player* player; /**< The player that last kicked the shell. */

	void onCollision(Sprite& sprite, Edge edge);
	void onCollision(Tile& tile, Edge edge);
	void onPreUpdate();
	void onRelease();
	void onStomp();
	void startMoving( Player* kicker );
	void stopMoving();
};
