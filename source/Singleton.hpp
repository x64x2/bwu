/*
 * Singleton.hpp
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


#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/**
* A global instance template for classes.
*
* @tparam T the class type to store in the Singleton.
*/
template <class T>
class Singleton
{
public:
	/**
	 * Create a new instance of the Singleton.
	 */
	static void createInstance()
	{
		instance = new T();
	}

	/**
	 * Destroy the current instance of the Singleton.
	 */
	static void destroyInstance()
	{
		delete instance;
		instance = 0;
	}

	/**
	 * Get the current instance of the Singleton.
	 *
	 * @return a reference to the current instance.
	 */
	static T& getInstance()
	{
		return *instance;
	}

	/**
	 * Set the instance used by the Singleton.
	 *
	 * @param newInstance the new instance to use.
	 */
	static void setInstance(T* newInstance)
	{
		if( instance )
			destroyInstance();
		instance = newInstance;
	}

private:
	static T* instance;

	Singleton() {}
	~Singleton() {}
};

template <class T>
T* Singleton<T>::instance = 0;

#endif // SINGLETON_HPP
