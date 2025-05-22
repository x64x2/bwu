/*
 * game.cpp
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

#include "Game.hpp"
#include "GameState.hpp"
#include "Globals.hpp"


/* main file*/
Game::Game( GameState* initialState ) :
	frameCount(0)
{
	pushState( initialState );
}

Game::~Game()
{
	for( std::list<GameState*>::iterator it = stateStack.begin(); it != stateStack.end(); ++it )
	{
		delete (*it);
	}

	for( std::list<GameState*>::iterator it = deadStateList.begin(); it != deadStateList.end(); ++it )
	{
		delete (*it);
	}
}

int Game::getFrameCount() const
{
	return frameCount;
}

bool Game::isRunning() const
{
	return !(stateStack.empty());
}

void Game::popState()
{
	deadStateList.push_back( stateStack.back() );
	stateStack.pop_back();
	if( !stateStack.empty() )
	{
		stateStack.back()->onResume();
	}
}

void Game::pushState( GameState* state )
{
	stateStack.push_back(state);
	state->game = this;
}

void Game::quit()
{
	while( !stateStack.empty() )
	{
		popState();
	}
}

void Game::switchState( GameState* state )
{
	popState();
	pushState(state);
}

void Game::update()
{
	stateStack.back()->update();

	for( std::list<GameState*>::iterator it = deadStateList.begin(); it != deadStateList.end(); ++it )
	{
		delete (*it);
	}
	deadStateList.clear();

	if( stateStack.empty() )
	{
		return;
	}
}
