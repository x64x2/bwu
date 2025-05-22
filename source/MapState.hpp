/*
 * MapState.hpp
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

#include "GameState.hpp"
#ifndef MAPSTATE_HPP
#define MAPSTATE_HPP

class Map;
class World;

/**
 * The game state that handles the player interacting with the world map.
 */
class MapState : public GameState
{
public:
	MapState();
	~MapState();

	void update();

private:
	Map* map;
	int node;
	int fadeInProgress;
	int fadeOutProgress;

	void input();
	void onResume();
	void render() const;
};
