/*
 * fireball.cpp
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



#include <cmath>
#include "DamageBlock.hpp"
#include "Enemy.hpp"
#include "Fireball.hpp"
#include "Globals.hpp"
#include "Particle.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "World.hpp"

Fireball::Fireball(Player* player, double xVelocity) :
	player(player)
{
	setWidth(FIREBALL_SIZE);
	setHeight(FIREBALL_SIZE);
	setAnimation("fireball");
	setXVelocity(xVelocity);
}

void Fireball::onCollision(Sprite& sprite, Edge edge)
{
	Enemy* enemy = dynamic_cast<Enemy*>(&sprite);
	if( enemy  != nullptr )
	{
		if( !enemy->isFireproof() )
		{
			enemy->hit();
			playSound("kick");
			player->scorePoints(MULTIPLE_KILL_POINT_SEQUENCE[0], sprite.getX() + sprite.getWidth(), sprite.getY());
		}
		else
		{
			playSound("bump");
		}
		kill();
	}
}

void Fireball::onCollision(Tile& tile, Edge edge)
{
	// Destroy fireballs when they hit damage blocks
	DamageBlock* block = dynamic_cast<DamageBlock*>(&tile);
	if( block != nullptr )
	{
		kill();
		playSound("bump");
		return;
	}

	if( edge == EDGE_BOTTOM )
	{
		setYVelocity(BOUNCE_VELOCITY);
	}
	else if( edge == EDGE_TOP && tile.getCollisionBehavior() != TILE_PLATFORM )
	{
		setYVelocity(-getYVelocity());
	}
	else if( (edge == EDGE_LEFT || edge == EDGE_RIGHT) && tile.getCollisionBehavior() == TILE_SOLID )
	{
		kill();
		playSound("bump");
	}
}

void Fireball::onDestroy()
{
	player->decrementFireballCount();

	// Create a smoke effect
	const Animation* a = getAnimation("fireball_smoke");
	Particle* p = new Particle(a, true);
	p->setCenterX(getCenterX());
	p->setCenterY(getCenterY());
	p->setGravityEnabled(false);
	getWorld().addSprite(p);
}

void Fireball::onPostUpdate()
{
	if( !isFlying() )
	{
		setYVelocity(BOUNCE_VELOCITY);
	}
}

void Fireball::onPreUpdate()
{
	if( std::fabs(getX() - player->getX()) > FIREBALL_DEATH_DISTANCE )
	{
		kill();
	}
}
