
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

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <list>
#include <vector>
/**
 * Buttons used for input.
 */
enum InputButton
{
	BUTTON_A,
	BUTTON_B,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_UP,
	BUTTON_SELECT,
	BUTTON_START,
};

/**
 * Handles input from the user and translates it into virtual button presses
 * for controllers.
 *
 */
class InputManager
{
public:
	/**
	 * An interface for listeners to Controller events.
	 */
	class ControllerListener
	{
	public:
		virtual ~ControllerListener() {}

		/**
		 * Event fired when a button state changes.
		 */
		virtual void onButtonToggle( InputButton button, bool pressed )=0;
	};

	/**
	 * A virtual controller.
	 */
	class Controller
	{
		friend class InputManager;
	public:
		Controller();

		/**
		 * Add a controller listener.
		 */
		void addListener( ControllerListener& listener );

		/**
		 * Returns true if a button is pressed.
		 */
		bool getButtonState( InputButton button ) const;

		/**
		 * Map the axis of a joystick to a button.
		 */
		void mapJoystickAxis( int joystick, int joystickAxis, int sign, InputButton button );

		/**
		 * Map a button on a joystick to a virtual controller button.
		 */
		void mapJoystickButton( int joystick, int joystickButton, InputButton button );

		/**
		 * Map a key to a virtual controller button.
		 *
		 * @param key the key to map.
		 * @param button the button to map the key to.
		 */
		void mapKey( SDL_Scancode key, InputButton button );

		/**
		 * Remove a controller listener.
		 */
		void removeListener( ControllerListener& listener );

	private:
		enum ButtonMappingType
		{
			MAP_KEY,
			MAP_JOYSTICK_BUTTON,
			MAP_JOYSTICK_AXIS
		};

		struct ButtonMapping
		{
			ButtonMappingType type;
			int joystick;
			int axisSign;
			union
			{
				int button;
				int axis;
			};
		};

		ButtonMapping buttonMappings[NUM_BUTTONS];
		bool buttonStates[NUM_BUTTONS];
		std::list<ControllerListener*> listeners;
	};

	InputManager();
	~InputManager();

	/**
	 * Add a controller to be managed.
	 *
	 * @return the id of the controller.
	 */
	int addController( Controller* controller );

	/**
	 * Get the controller with a particular id.
	 */
	Controller* getController( int id );

	/**
	 * Update input for all controllers.
	 */
	void update();

private:
	std::vector<Controller*> controllers;
};
