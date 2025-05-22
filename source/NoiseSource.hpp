/*
 * NoiseSource.hpp
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
 * A source of pseudorandom noise.
 *
 * @tparam T the type to use for noise that is returned.
 */
 
 #ifndef NOISESOURCE_HPP
#define NOISESOURCE_HPP

template <typename T>
class NoiseSource
{
public:
	virtual ~NoiseSource() {}

	/**
	 * Fractal brownian motion.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 * @param z the z coordinate.
	 * @param octaves the number of iterations to perform.
	 * @param persistance the amount to modify amplitude by each octave.
	 * @param lacunarity the amount to modify frequency by each octave.
	 * @return the value of FBM with the given parameters.
	 */
	T fbm(T x, T y = 0, T z = 0, unsigned int octaves = 4, T persistance = 0.5, T lacunarity = 2) const
	{
		T total = 0;
		T amplitude = persistance;
		T frequency = 1;
		T max = 0;

		for( unsigned int i = 0; i < octaves; i++ )
		{
			total += noise((T)x * frequency, (T)y * frequency, (T)z * frequency) * amplitude;
			max += amplitude;
			frequency *= lacunarity;
			amplitude *= persistance;
		}

		return total / max;
	}

	/**
	 * Get noise at (x, y, z).
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 * @param z the z coordinate.
	 * @return the noise value at (x, y, z).
	 */
	virtual T noise(T x, T y = 0, T z = 0) const =0;
};

