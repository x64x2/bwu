/*
 * globals.hpp
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
 * @brief defines functions, macros, and constants dealing with global
 * variables and singleton objects.
 */
 


#include "InputManager.hpp"
#include "GameSession.hpp"
#include "Logger.hpp"
#include "ResourceManager.hpp"
#include "Settings.hpp"
#include "Singleton.hpp"

#define GAME_TITLE "lesu"

#ifndef GLOBALS_HPP
#define GLOBALS_HPP


/* global macros*/
// useful macros for Resource management
#define RESOURCE_MANAGER (Singleton<ResourceManager>::getInstance())
#define GET_RESOURCE_GROUP(name) (Singleton<ResourceManager>::getInstance().getResourceGroup(name))
#define DEFAULT_RESOURCE_GROUP_NAME "default"
#define DEFAULT_RESOURCE_GROUP (GET_RESOURCE_GROUP(DEFAULT_RESOURCE_GROUP_NAME))

// Note: do not use these macros within entity classes unless the resources should not be overridden
#define GET_ANIMATION(name) (DEFAULT_RESOURCE_GROUP->getAnimation(name))
#define GET_BACKGROUND(name) (DEFAULT_RESOURCE_GROUP->getBackground(name))
#define GET_FONT(name) (DEFAULT_RESOURCE_GROUP->getFont(name))
#define GET_MUSIC(name) (DEFAULT_RESOURCE_GROUP->getMusic(name))
#define GET_TILESET(name) (DEFAULT_RESOURCE_GROUP->getTileset(name))
#define PLAY_MUSIC(name,loop) (DEFAULT_RESOURCE_GROUP->playMusic(name,loop))
#define PLAY_SOUND(name) (DEFAULT_RESOURCE_GROUP->playSound(name))
#define PLAY_SOUND_CHANNEL(name,channel) (DEFAULT_RESOURCE_GROUP->playSound(name,channel))

// Useful macro for logging
#define LOG (Singleton<Logger>::getInstance())

// Useful singleton access
#define FPS_MANAGER (Singleton<FpsManager>::getInstance())
#define GAME_SESSION (Singleton<GameSession>::getInstance())
#define INPUT_MANAGER (Singleton<InputManager>::getInstance())
#define SETTINGS (Singleton<Settings>::getInstance())

/* global variable*/
///@todo we need a singleton for rendering stuff. Move this once we have one
extern void* window; /**< Handle to the SDL2 window. */




// @richelbilderbeek: changed const to constexpr
static constexpr double GAME_FPS = 60.0;          /**< Frames per second that the Game runs at. */
constexpr double GAME_DELTA = 1.0 / GAME_FPS; /**< Delta (change in time) between each frame. */

static const int UNIT_SIZE = 16;     /**< The number of pixels in one world unit. */
static const int HALF_UNIT_SIZE = 8; /**< The number of pixels in a half world unit. */

/**
 * The point sequence for when the player stomps multiple enemies or kills
 * multiple enemies with a kicking object. -1 signifies the end of the array.
 */
static const int MULTIPLE_KILL_POINT_SEQUENCE[] =
{
	100,
	200,
	400,
	800,
	1000,
	2000,
	4000,
	8000,
	-1
};

static constexpr int FADE_DURATION = 60; /**< The number of frames in a fade in/fade out when the player enters/leaves a level. */

/* global function*/
/**
 * Create instances of global objects.
 */
void createGlobals();

/**
 * Destroy all instances of global objects.
 */
void destroyGlobals();

