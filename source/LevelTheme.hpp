/*
 * LevelTheme.hpp
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

#include <string>
#include <vector>

#include "EntityTypes.hpp"

v
class Background;
class Music;
class ResourceManager;

/**
 * Determines the theming (resources used) when generating a random Level.
 */
struct LevelTheme
{
	std::vector<const Background*> backgrounds;
	std::vector<const Music*> musics;
	std::vector<const ResourceManager*> spriteTypes[NUM_SPRITE_TYPES];
	std::vector<const ResourceManager*> tileTypes[NUM_TILE_TYPES];

	/**
	 * Add an entity type to the theme to be randomly selected.
	 *
	 * @param name the name of the entity type.
	 * @param resourceGroup the ResourceManager for the entity to use.
	 */
	void addEntityType( const std::string& name, const ResourceManager* resourceGroup );

	/**
	 * Inherit the theming resource groups from a parent level theme. Any
	 * type that the parent has a resource specified for that this
	 * instance lacks will be copied over.
	 */
	void inherit( const LevelTheme& parent );
};

#endif // LEVELTHEME_HPP
