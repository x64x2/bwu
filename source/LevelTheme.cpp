/*
 * LevelTheme.cpp
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



#include "Globals.hpp"
#include "LevelTheme.hpp"

void LevelTheme::addEntityType( const std::string& name, const ResourceManager* resourceGroup )
{
	// Is is a sprite?
	SpriteType spriteType = getSpriteType(name);
	if( spriteType != TYPE_SPRITE_NULL )
	{
		spriteTypes[spriteType].push_back(resourceGroup);
		return;
	}

	// Is it a tile?
	TileType tileType = getTileType(name);
	if( tileType != TYPE_TILE_NULL )
	{
		tileTypes[tileType].push_back(resourceGroup);
		return;
	}

	// We didn't recognize the type
	LOG << "Warning: attempted to add an entity type to an LevelTheme with invalid name \"" << name << "\".\n";
}

void LevelTheme::inherit( const LevelTheme& parent )
{
	for( int i = 0; i < NUM_SPRITE_TYPES; i++ )
	{
		if( spriteTypes[i].empty() )
		{
			spriteTypes[i] = parent.spriteTypes[i];
		}
	}
	for( int i = 0; i < NUM_TILE_TYPES; i++ )
	{
		if( tileTypes[i].empty() )
		{
			tileTypes[i] = parent.tileTypes[i];
		}
	}
}
