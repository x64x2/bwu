
/*
 * enums.hpp
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

#include "map.hpp"
#ifndef EPISODE_HPP
#define EPISODE_HPP

class Level;
class LevelGenerator;
struct LevelTheme;

/**
 * A playable episode of the game, consisting of Levels and Maps.
 */
class Episode
{
public:
	~Episode();

	/**
	 * Add a level to the Episode.
	 *
	 * @param levelId the ID number of the Level.
	 * @param level the Level to add.
	 */
	void addLevel( int levelId, Level* level );

	/**
	 * Get the Level from the Episode by its ID.
	 *
	 * @param levelID the ID of the Level.
	 */
	const Level* getLevel( int levelId );

private:
	std::map< int, Level* > levels;
};

#endif // EPISODE_HPP
