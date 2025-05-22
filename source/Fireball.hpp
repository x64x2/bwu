/*
 * fireball.hpp
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
#ifndef FIREBALL_HPP
#define FIREBALL_HPP

class Player;

/**
 * A fireball thrown by the Player.
 */
class Fireball : public Sprite
{
public:
	static constexpr double BOUNCE_VELOCITY = 16.0;  /**< The bounce velocity of a Fireball. */
	static constexpr double FIREBALL_SIZE = 0.5;     /**< The size of a Fireball. */
	static constexpr double X_VELOCITY = 12.0;       /**< The x velocity of a Fireball. */
	static constexpr double FIREBALL_DEATH_DISTANCE = 32.0; /**< The maximum distance a fireball can be from the player before it disintegrates. */

	/**
	 * Create a new fireball.
	 *
	 * @param player the Player that threw the fireball.
	 * @param xVelocity the starting x velocity of the fireball.
	 */
	Fireball(Player* player, double xVelocity);

private:
	Player* player;

	void onCollision(Sprite& sprite, Edge edge);
	void onCollision(Tile& tile, Edge edge);
	void onDestroy();
	void onPostUpdate();
	void onPreUpdate();
};

#endif // FIREBALL_HPP
