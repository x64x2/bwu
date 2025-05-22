/*
 * Music.hpp
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
#include <SDL2/SDL_mixer.h>

#ifndef MUSIC_HPP
#define MUSIC_HPP
/**
 * A music track.
 */
class Music
{
public:
	/**
	 * Load a music track from a file.
	 *
	 * @param fileName the name of the file.
	 */
	Music(const std::string& fileName);

	~Music();

	/**
	 * Check if the music track has been loaded successfully.
	 */
	bool isLoaded() const;

	/**
	 * Play the music track.
	 *
	 * @param loop whether the track should loop infinitely or not.
	 */
	void play(bool loop) const;

private:
	Mix_Music* music;
};
