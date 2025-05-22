/*
 * enemy.cpp
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
#include "Particle.hpp"
#include "Shell.hpp"
#include "Tile.hpp"
#include "World.hpp"

Enemy::Enemy() :
	bounceOffEnemiesEnabled(true),
	fireproof(false),
	kickable(false),
	stompable(true),
	velocityOrientationEnabled(true),
	defaultXVelocity(0.0)
{
}

void Enemy::bump()
{
	onBump();
}

void Enemy::createParticleAndDie()
{
	// Create an upside-down enemy particle
	Sprite* particle = new Particle(getActiveAnimation());
	particle->disableCollisions();
	particle->setOrientation(DOWN);
	particle->setYVelocity(BUMP_VELOCITY);
	getWorld().addSprite(getX(), getY(), particle);
	kill();
}

void Enemy::hit()
{
	onHit();
}

bool Enemy::isFireproof() const
{
	return fireproof;
}

bool Enemy::isKickable() const
{
	return kickable;
}

bool Enemy::isStompable() const
{
	return stompable;
}

void Enemy::onBump()
{
	createParticleAndDie();
}

void Enemy::onCollision(Sprite& sprite, Edge edge)
{
	if( !bounceOffEnemiesEnabled )
	{
		return;
	}

	Enemy* enemy = dynamic_cast<Enemy*>(&sprite);
	if( enemy != nullptr && enemy->bounceOffEnemiesEnabled )
	{
		// Head-on collision
		if( ((getCenterX() < sprite.getCenterX() && getXVelocity() > 0.0 && sprite.getXVelocity() <= 0.0) ||
			(getCenterX() > sprite.getCenterX() && getXVelocity() < 0.0 && sprite.getXVelocity() >= 0.0)) )
		{
			setXVelocity(-getXVelocity());
			if( dynamic_cast<Shell*>(&sprite) == nullptr )
			{
				sprite.setXVelocity(-sprite.getXVelocity());
			}
		}
		// Hitting another enemy in the back
		else if( (getCenterX() < sprite.getCenterX() && getXVelocity() > 0.0) ||
			(getCenterX() > sprite.getCenterX() && getXVelocity() < 0.0) )
		{
			setXVelocity(-getXVelocity());
		}
	}
}

void Enemy::onCollision(Tile& tile, Edge edge)
{
	bounceOffWalls(tile, edge);
}

void Enemy::onHit()
{
	createParticleAndDie();
}

void Enemy::onPreUpdate()
{
	//avoidCliffs();

	// Make sure that we don't get stuck
	if( getXVelocity() == 0.0 )
	{
		setXVelocity(defaultXVelocity);
	}
}

void Enemy::onRender()
{
	if( !velocityOrientationEnabled )
	{
		return;
	}

	if( getXVelocity() > 0.0 )
	{
		setOrientation(RIGHT);
	}
	else if( getXVelocity() < 0.0 )
	{
		setOrientation(LEFT);
	}
}

void Enemy::onStomp()
{
	// Create an upside-down enemy particle
	Particle* p = new Particle(getActiveAnimation());
	p->disableCollisions();
	p->setCenterX( getCenterX() );
	p->setCenterY( getCenterY() );
	p->setOrientation(DOWN);
	if( getXVelocity() > 0.0 )
	{
		p->setOrientation(RIGHT);
	}
	else
	{
		p->setOrientation(LEFT);
	}
	getWorld().addSprite(p);

	// Kill the enemy
	kill();
}

void Enemy::setBounceOffEnemiesEnabled( bool enabled )
{
	bounceOffEnemiesEnabled = enabled;
}

void Enemy::setDefaultXVelocity( double velocity )
{
	defaultXVelocity = velocity;
	setXVelocity(velocity);
}

void Enemy::setFireproof( bool fireproof )
{
	this->fireproof = fireproof;
}

void Enemy::setKickable(bool kickable)
{
	this->kickable = kickable;
}

void Enemy::setStompable(bool stompable)
{
	this->stompable = stompable;
}

void Enemy::setVelocityOrientationEnabled( bool enabled )
{
	velocityOrientationEnabled = enabled;
}

void Enemy::stomp()
{
	onStomp();
}
