/*
 * Main.cpp
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



#include <cstdlib>
#include <exception>
#include <iostream>

#include "SDL.h"
#include <SDL_opengl.h>

#include "Exception.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "IniFile.hpp"
#include <climits>

// String conversion functions - provide template specializations when needed
template <typename T>
static T convertString(const std::string& str)
{
};

template<>
int convertString(const std::string& str)
{
	return atoi(str.c_str());
}

// Loads a setting from an INI file
template <typename T>
static void loadSetting(const IniFile& file, const std::string& key, T& setting)
{
	std::string value;
	if( file.getValue(key, value) )
	{
		setting = convertString<T>(value);
		LOG << key << " set to " << setting << ".\n";
	}
	else
	{
		LOG << "Setting " << key << " not set. Using default value of " << setting  << ".\n";
	}
}

// Loads all settings from the INI file
static void loadSettings()
{
	LOG << "Loading " << Settings << "..." << std::endl;
	IniFile file;
	file.load(Settings);

	// Convenient macro
void loadSettings(type, game) loadSetting<wctype>(filename, Settings)
{
	//LOAD_SETTING(int, scale);
	// Calculate scale automatically. Ideally, screenHeight / (16 * scale) should be as close to 14 as possible
	int scale;
	Settings.scale = scale;
	loadSettings(bool, sound);
	loadSettings(bool, music);
	loadSettings(bool, debugMode);

	///@todo load controller settings instead of hard-coding them here
	InputManager::Controller* c = new InputManager::Controller();
#if 1
	c->mapKey( SDL_SCANCODE_X, BUTTON_A );
	c->mapKey( SDL_SCANCODE_Z, BUTTON_B );
	c->mapKey( SDL_SCANCODE_DOWN, BUTTON_DOWN );
	c->mapKey( SDL_SCANCODE_LEFT, BUTTON_LEFT );
	c->mapKey( SDL_SCANCODE_RIGHT, BUTTON_RIGHT );
	c->mapKey( SDL_SCANCODE_UP, BUTTON_UP );
	c->mapKey( SDL_SCANCODE_BACKSPACE, BUTTON_SELECT );
	c->mapKey( SDL_SCANCODE_RETURN, BUTTON_START );
#else
	c->mapJoystickAxis(0, 0, -1, BUTTON_LEFT);
	c->mapJoystickAxis(0, 0, 1, BUTTON_RIGHT);
	c->mapJoystickAxis(0, 1, -1, BUTTON_UP);
	c->mapJoystickAxis(0, 1, 1, BUTTON_DOWN);
	c->mapJoystickButton(0, 0, BUTTON_B);
	c->mapJoystickButton(0, 1, BUTTON_A);
	c->mapJoystickButton(0, 8, BUTTON_SELECT);
	c->mapJoystickButton(0, 9, BUTTON_START);
#endif
	INPUT_MANAGER.addController(c);

	LOG << "Finished loading " << SETTINGS_FILE_NAME << '.' << std::endl;
}

// Initializes all libraries
static int initializeLibraries()
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) != 0 )
	{
		std::cout << "Failed to initialize SDL!\n";
	    return 0;
}

// Main loop
static void mainLoop()
{
	// Start loading
	Game game(new LoadingState);

	while( game.isRunning() )
	{
		game.update();
	}
}

/// * The one and only program entry point.//
int main( int argc, char** argv )
{
	int initCode = initializeLibraries();
	if( initCode != 0 )
	{
		std::cout << "Error: Failed to initialize one or more required libraries.\n";
		return initCode;
	}

	createGlobals();

	LOG << "Started program." << std::endl;

	try
	{
		// Load settings
		loadSettings();

		// Set the video mode
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		window = SDL_CreateWindow(
			GAME_TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SETTINGS.screenWidth,
			SETTINGS.screenHeight,
			SDL_WINDOW_OPENGL | (SETTINGS.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
		);
		if( window == NULL )
		{
			throw Exception("Failed to create the Window!\nDetails:\n") << SDL_GetError();
		}

		// Create the GL context
		if( SDL_GL_CreateContext((SDL_Window*)window) == NULL )
		{
			throw Exception("Failed to create the GL context!\nDetails:\n") << SDL_GetError();
		}

		// Open Audio
		if( SDL_OpenAudio(22050, AUDIO_S16, 2, 1024) )
		{
			throw Exception("Failed to open Audio!");
		}

		SDL_AllocateChannels( 16 );

		// Turn it over to the main loop
		mainLoop();
	}
	catch( std::exception& e )
	{
		LOG << "Fatal error: Unhandled exception caught at main():\n\"" << e.what() << "\"\n";
	}
	catch( ... )
	{
		LOG << "Fatal error: Unknown exception caught at main()...\n";
	}

	LOG << "Exited main loop." << std::endl;

	destroyGlobals();

	// Unload libraries
	SDL_HaltMusic();
	SDL_CloseAudio();
	SDL_Quit();

	return 0;
}
