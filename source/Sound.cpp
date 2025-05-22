/*
 * Sound.cpp
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


#include "Sound.hpp"

Sound::Sound( const std::string& fileName )
{
	chunk = Mix_LoadWAV(fileName.c_str());
}

Sound::~Sound()
{
	Mix_FreeChunk(chunk);
}

bool Sound::isLoaded() const
{
	return (chunk != nullptr);
}

void Sound::play( int channel ) const
{
	Mix_PlayChannel(channel, chunk, 0);
}
