/*
 * Settings.hpp
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



#define SETTINGS_HPP

#define SETTINGS_FILE_NAME "settings.ini"

/**
 * Settings loaded from the configuration file.
 */
struct Settings
{
	int screenWidth;  /**< The width of the screen, in pixels. */
	int screenHeight; /**< The height of the screen, in pixels. */
	int scale;        /**< The zoom level for rendering. */
	bool fullscreen;  /**< Full screen on/off. */
	bool sound;       /**< Sound effects on/off. */
	bool music;       /**< Music on/off. */
	bool debugMode;   /**< Debug mode on/off. */

	/**
	 * Initializes with default settings.
	 */
	Settings();

	/**
	 * Get the height of the screen in pixels with scale taken into consideration.
	 */
	int getRenderedScreenHeight() const;

	/**
	 * Get the width of the screen in pixels with scale taken into consideration.
	 */
	int getRenderedScreenWidth() const;
};
