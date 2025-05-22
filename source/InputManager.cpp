/*
 * inputmanager.cpp
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
#include "InputManager.hpp"

InputManager::Controller::Controller()
{
	for( int i = 0; i < NUM_BUTTONS; i++ )
	{
		buttonStates[i] = false;
	}
}

void InputManager::Controller::addListener( ControllerListener& listener )
{
	listeners.push_back(&listener);
}

bool InputManager::Controller::getButtonState( InputButton button ) const
{
	return buttonStates[button];
}

void InputManager::Controller::mapJoystickAxis( int joystick, int joystickAxis, int sign, InputButton button )
{
	ButtonMapping& b = buttonMappings[button];
	b.type = MAP_JOYSTICK_AXIS;
	b.joystick = joystick;
	b.axis = joystickAxis;
	b.axisSign = sign;
}

void InputManager::Controller::mapJoystickButton( int joystick, int joystickButton, InputButton button )
{
	ButtonMapping& b = buttonMappings[button];
	b.type = MAP_JOYSTICK_BUTTON;
	b.joystick = joystick;
	b.button = joystickButton;
}

void InputManager::Controller::mapKey( SDL_Scancode key, InputButton button )
{
	ButtonMapping& b = buttonMappings[button];
	b.key = key;
	b.type = MAP_KEY;
}

void InputManager::Controller::removeListener( ControllerListener& listener )
{
	listeners.remove(&listener);
}

InputManager::InputManager()
{
	// Open all joysticks for use
	for( int i = 0; i < SDL_NumJoysticks(); i++ )
	{
		SDL_Joystick* joy = SDL_JoystickOpen(i);
		if( joy )
		{
			LOG << "Opened joystick on device index " << i << ".\n";
		}
		else
		{
			LOG << "Warning: Failed to open joystick on device index " << i << "!\n";
		}
		joysticks.push_back(joy);
	}
}

InputManager::~InputManager()
{
	for( auto c : controllers )
	{
		delete c;
	}

	for( auto j : joysticks )
	{
		SDL_JoystickClose(j);
	}
}

int InputManager::addController( Controller* controller )
{
	controllers.push_back(controller);
	return controllers.size() - 1;
}

InputManager::Controller* InputManager::getController(int id)
{
	return controllers[id];
}

void InputManager::update()
{
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	SDL_JoystickUpdate();
	for( auto c : controllers )
	{
		for( int i = 0; i < NUM_BUTTONS; i++ )
		{
			Controller::ButtonMapping& b = c->buttonMappings[i];
			bool state = false;
			if( b.type == Controller::MAP_KEY )
			{
				state = keystates[b.key];
			}
			else if( b.type == Controller::MAP_JOYSTICK_AXIS )
			{
				int s = (b.axisSign > 0 ? 1 : -1);
				Sint16 axisState = SDL_JoystickGetAxis(joysticks[b.joystick], b.axis);
				if( s < 0 )
				{
					state = (axisState < -16384);
				}
				else
				{
					state = (axisState > 16384);
				}
			}
			else if( b.type == Controller::MAP_JOYSTICK_BUTTON )
			{
				state = (SDL_JoystickGetButton(joysticks[b.joystick], b.button) != 0);
			}

			if( state != c->buttonStates[i] )
			{
				for( auto l : c->listeners )
				{
					l->onButtonToggle( static_cast<InputButton>(i), state );
				}
			}
			c->buttonStates[i] = state;
		}
	}
}
