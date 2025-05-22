/*
 * damageblock.cpp
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


#include "DamageBlock.hpp"
#include "Player.hpp"

DamageBlock::DamageBlock( bool fatal ) :
	Tile(fatal ? TILE_NONE : TILE_SOLID),
	fatal(fatal)
{
	if( fatal )
	{
		setAnimation("damage_block_fatal");
	}
	else
	{
		setAnimation("damage_block");
	}
}

void DamageBlock::onCollision( Sprite& sprite, Edge edge )
{
	// Have the player take damage when colliding with the damage block
	Player* player = dynamic_cast<Player*>(&sprite);
	if( player != nullptr )
	{
		player->takeDamage( fatal );
	}
}
