/*
 * Particle.hpp
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
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

/**
 * An animated effect sprite.
 */
class Particle : public Sprite
{
public:
	/**
	 * Create a new particle with infinite duration.
	 *
	 * @param animation the Animation to use for rendering.
	 * @param destroyOnAnimationEnd whether the particle should be destroyed when its animation is complete.
	 */
	Particle( const Animation* animation, bool destroyOnAnimationEnd = false );

	/**
	 * Create a new particle with an initial delay before it plays its animation.
	 *
	 * @param animation the Animation to use for rendering.
	 * @param initialDelay the initial delay, in seconds, to wait before playing the animation after spawning.
	 * @param destroyOnAnimationEnd whether the particle should be destroyed when its animation is complete.
	 */
	Particle( const Animation* animation, double initialDelay, bool destroyOnAnimationEnd );

	/**
	 * Create a new particle.
	 *
	 * @param animation the Animation to use for rendering.
	 * @param duration the length of time that the particle will live for.
	 */
	Particle( const Animation* animation, double duration );

private:
	const Animation* animationToPlay;
	bool destroyOnAnimationEnd;
	double duration;
	double initialDelay;

	void initialize( const Animation* animation );
	void onPlayAnimationEnd();
	void onPreUpdate();
};
