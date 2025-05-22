/*
 * entitytypes.hpp
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


/**
 * @file
 * @brief defines Entity type enumerations and functions for conversion to them.
 */

#include <string>

#ifndef ENTITYTYPES_HPP
#define ENTITYTYPES_HPP

/**
 * The types of sprite objects that can be created.
 *
 * @note whenever a new enumeration constant is added, be sure to add it
 * to the string switch in EntityTypes.cpp. Also, add the
 * entity to the createSprite() function in the Level class.
 * Also, add it to the ignore list in Level::Level() if appropriate.
 */
enum SpriteType
{
	TYPE_SPRITE_NULL,

	TYPE_BEETLE,
	TYPE_COIN,
	TYPE_FLOWER,
	TYPE_GOOMBA,
	TYPE_GROWING_LADDER,
	TYPE_HAMMER_BRO,
	TYPE_KOOPA,
	TYPE_LAKITU,
	TYPE_LEAF,
	TYPE_LEVEL_END,
	TYPE_MUSHROOM,
	TYPE_MUSHROOM_1UP,
	TYPE_PARATROOPA,
	TYPE_PLANT,
	TYPE_SHELL,
	TYPE_SPINY,
	TYPE_STAR,

	NUM_SPRITE_TYPES
};

/**
 * The types of tile objects that can be created.
 *
 * @note whenever a new enumeration constant is added, be sure to add it
 * to the string switch in EntityTypes.cpp. Also, add the
 * entity to the createTile() function in the Level class.
 * * Also, add it to the ignore list in Level::Level() if appropriate.
 */
enum TileType
{
	TYPE_TILE_NULL,

	TYPE_BLASTER,
	TYPE_BLOCK,
	TYPE_BRICK,
	TYPE_DAMAGE_BLOCK,
	TYPE_DAMAGE_BLOCK_FATAL,
	TYPE_GROUND,
	TYPE_LADDER,
	TYPE_LEDGE,
	TYPE_PIPE_DOWN,
	TYPE_PIPE_LEFT,
	TYPE_PIPE_RIGHT,
	TYPE_PIPE_UP,
	TYPE_SLOPE_DOWN,
	TYPE_SLOPE_UP,
	TYPE_QUESTION_BLOCK,
	TYPE_WATER,

	NUM_TILE_TYPES
};

SpriteType getSpriteType( const std::string& type );

TileType getTileType( const std::string& type );

#endif // ENTITYTYPES_HPP
