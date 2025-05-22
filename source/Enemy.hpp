/*
 * enemy.hpp
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

#ifndef ENEMY_HPP
#define ENEMY_HPP

static constexpr double BUMP_VELOCITY = 10.0; /**< The initial y velocity of a bumped enemy. */

/**
 * Base class for enemies of the player.
 */
class Enemy : public Sprite
{
public:
	Enemy();

	/**
	 * Bump an enemy from below.
	 */
	void bump();

	/**
	 * Hit the enemy with a shell (or another forceful object).
	 */
	void hit();

	/**
	 * Check if the enemy is resistant to fireballs.
	 */
	bool isFireproof() const;

	/**
	 * Check if an Enemy can be safely kicked from the side.
	 */
	bool isKickable() const;

	/**
	 * Check if an Enemy can be stomped (not necessarily if it will be
	 * dangerous if attempted).
	 */
	bool isStompable() const;

	/**
	 * Stomp an enemy from above.
	 */
	void stomp();

protected:
	virtual void onBump();
	virtual void onCollision(Sprite& sprite, Edge edge);
	virtual void onCollision(Tile& tile, Edge edge);
	virtual void onHit();
	virtual void onPreUpdate();
	void onRender();
	virtual void onStomp();
	void setBounceOffEnemiesEnabled( bool enabled );

	/**
	 * Set the default x velocity to set the enemy to if no movement is
	 * detected. This fixes some movement bugs, so be sure to use it
	 * when creating new enemies.
	 */
	void setDefaultXVelocity( double velocity );

	void setFireproof( bool fireproof );
	void setKickable( bool kickable );
	void setStompable( bool stompable );
	void setVelocityOrientationEnabled( bool enabled );

private:
	bool bounceOffEnemiesEnabled;
	bool fireproof;
	bool kickable;
	bool stompable;
	bool velocityOrientationEnabled;
	double defaultXVelocity;

	void createParticleAndDie();
};

