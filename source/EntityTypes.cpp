/*
 * entitytypes.cpp
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

#include "EntityTypes.hpp"
#include "StringSwitch.hpp"

static bool stringSwitchesInitialized = false;
static StringSwitch spriteNameSwitch;
static StringSwitch tileNameSwitch;

static void initializeStringSwitches()
{
	spriteNameSwitch.setDefaultValue( TYPE_SPRITE_NULL );
	spriteNameSwitch.addString( "beetle", TYPE_BEETLE );
	spriteNameSwitch.addString( "coin", TYPE_COIN );
	spriteNameSwitch.addString( "flower", TYPE_FLOWER );
	spriteNameSwitch.addString( "goomba", TYPE_GOOMBA );
	spriteNameSwitch.addString( "growing_ladder", TYPE_GROWING_LADDER );
	spriteNameSwitch.addString( "hammer_bro", TYPE_HAMMER_BRO );
	spriteNameSwitch.addString( "koopa", TYPE_KOOPA );
	spriteNameSwitch.addString( "lakitu", TYPE_LAKITU );
	spriteNameSwitch.addString( "leaf", TYPE_LEAF );
	spriteNameSwitch.addString( "level_end", TYPE_LEVEL_END );
	spriteNameSwitch.addString( "mushroom", TYPE_MUSHROOM );
	spriteNameSwitch.addString( "mushroom_1up", TYPE_MUSHROOM_1UP );
	spriteNameSwitch.addString( "paratroopa", TYPE_PARATROOPA );
	spriteNameSwitch.addString( "plant", TYPE_PLANT );
	spriteNameSwitch.addString( "shell", TYPE_SHELL );
	spriteNameSwitch.addString( "spiny", TYPE_SPINY );
	spriteNameSwitch.addString( "star", TYPE_STAR );

	tileNameSwitch.setDefaultValue( TYPE_TILE_NULL );
	tileNameSwitch.addString( "blaster", TYPE_BLASTER );
	tileNameSwitch.addString( "block", TYPE_BLOCK );
	tileNameSwitch.addString( "brick", TYPE_BRICK );
	tileNameSwitch.addString( "damage_block", TYPE_DAMAGE_BLOCK );
	tileNameSwitch.addString( "damage_block_fatal", TYPE_DAMAGE_BLOCK_FATAL );
	tileNameSwitch.addString( "ground", TYPE_GROUND );
	tileNameSwitch.addString( "ladder", TYPE_LADDER );
	tileNameSwitch.addString( "ledge", TYPE_LEDGE );
	tileNameSwitch.addString( "pipe_down", TYPE_PIPE_DOWN );
	tileNameSwitch.addString( "pipe_left", TYPE_PIPE_LEFT );
	tileNameSwitch.addString( "pipe_right", TYPE_PIPE_RIGHT );
	tileNameSwitch.addString( "pipe_up", TYPE_PIPE_UP );
	tileNameSwitch.addString( "question_block", TYPE_QUESTION_BLOCK );
	tileNameSwitch.addString( "slope_down", TYPE_SLOPE_DOWN );
	tileNameSwitch.addString( "slope_up", TYPE_SLOPE_UP );
	tileNameSwitch.addString( "water", TYPE_WATER );

	stringSwitchesInitialized = true;
}

SpriteType getSpriteType( const std::string& type )
{
	if( !stringSwitchesInitialized )
	{
		initializeStringSwitches();
	}
	return static_cast<SpriteType>(spriteNameSwitch.test(type));
}

TileType getTileType( const std::string& type )
{
	if( !stringSwitchesInitialized )
	{
		initializeStringSwitches();
	}
	return static_cast<TileType>(tileNameSwitch.test(type));
}
