/*
 * episodes.hpp
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

#include "Episode.hpp"
#include "Exception.hpp"
#include "Globals.hpp"
#include "Level.hpp"

Episode::~Episode()
{
	for( auto it : levels )
	{
		delete it.second;
	}
}

void Episode::addLevel( int levelId, Level* level )
{
	auto it = levels.find( levelId );
	if( it == levels.end() )
	{
		levels[levelId] = level;
	}
	else
	{
		LOG << "Warning: tried to add a level to an episode using an ID that already existed.\n";
	}
}

const Level* Episode::getLevel( int levelId )
{
	auto it = levels.find( levelId );
	if( it != levels.end() )
	{
		Level* level = (*it).second;
		if( level != nullptr )
		{
			return level;
		}
		else
		{
			throw Exception() << "Invalid level ID [" << levelId << "] passed to Episode::getLevel()";
		}
	}
	else
	{
		throw Exception() << "Invalid level ID [" << levelId << "] passed to Episode::getLevel()";
	}
}
