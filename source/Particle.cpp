/*
 * Particle.cpp
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


#include "Particle.hpp"

Particle::Particle( const Animation* animation, bool destroyOnAnimationEnd ) :
	destroyOnAnimationEnd(destroyOnAnimationEnd),
	duration(-1.0),
	initialDelay(-1.0)
{
	if( destroyOnAnimationEnd )
	{
		playAnimation(animation);
		duration = animation->getDuration();
	}
	else
	{
		playAnimation(animation, animation);
	}
	//initialize(animation);
}

Particle::Particle( const Animation* animation, double initialDelay, bool destroyOnAnimationEnd ) :
	animationToPlay(animation),
	destroyOnAnimationEnd(destroyOnAnimationEnd),
	duration(-1.0),
	initialDelay(initialDelay)
{
	initialize(animation); // for now, this is only used on coin sparkles.
}

Particle::Particle( const Animation* animation, double duration ) :
	destroyOnAnimationEnd(false),
	duration(duration),
	initialDelay(-1.0)
{
	playAnimation(animation, animation);
	//initialize(animation);
}

void Particle::initialize( const Animation* animation )
{
	// Set our size to be the size of the first frame of the animation
	if( animation != nullptr )
	{
		auto f = animation->getFrame(0);
		setWidth(f.width);
		setHeight(f.height);
	}
}

void Particle::onPlayAnimationEnd()
{
	if( destroyOnAnimationEnd )
	{
		kill();
	}
}

void Particle::onPreUpdate()
{
	// Kill the particle if it has been alive for the set duration
	if( duration != -1.0 && getAge() > duration )
	{
		kill();
	}

	// If we had an initial delay, play the animation once the delay is over
	if( initialDelay != -1.0 && getAge() > initialDelay )
	{
		if( destroyOnAnimationEnd )
		{
			playAnimation(animationToPlay);
			duration = animationToPlay->getDuration() + initialDelay;
		}
		else
		{
			playAnimation(animationToPlay, animationToPlay);
		}
		initialDelay = -1.0;
	}
}
