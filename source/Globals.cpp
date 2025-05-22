/*
 * globals.cpp
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

void* window = nullptr;

void createGlobals()
{
	Singleton<Logger>::setInstance(new Logger); // This always goes first
	Singleton<FpsManager>::setInstance(new FpsManager(GAME_FPS));
	Singleton<Settings>::createInstance();
	Singleton<ResourceManager>::createInstance();
	Singleton<InputManager>::createInstance();
	Singleton<GameSession>::createInstance();
}

void destroyGlobals()
{
	Singleton<GameSession>::destroyInstance();
	Singleton<InputManager>::destroyInstance();
	Singleton<Settings>::destroyInstance();
	Singleton<ResourceManager>::destroyInstance();
	Singleton<Logger>::destroyInstance(); // This always goes last
}
