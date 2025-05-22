/*
 * image.hpp
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
#include "Color.hpp"

#ifndef IMAGE_HPP
#define IMAGE_HPP

/**
 * An array of pixels.
 */
class Image 
{
public:
	/**
	 * Create an empty image.
	 *
	 * @param width the width of the image.
	 * @param height the height of the image.
	 */
	Image(int width, int height);

	/**
	 * Load an image from a file.
	 *
	 * @param fileName the name of the file to load from.
	 */
	Image(const std::string& fileName);

	~Image();

	/**
	 * Blit another image onto the image.
	 *
	 * @param source the source Image.
	 * @param x the left coordinate.
	 * @param y the top coordinate.
	 * @note This does not check bounds prior to blitting. Be careful.
	 */
	void blit( const Image& source, int x, int y );

	/**
	 * Get the height of the image.
	 */
	int getHeight() const;

	/**
	 * Get a pixel of the Image.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 */
	Color getPixel(int x, int y) const;

	/**
	 * Get the width of the image.
	 */
	int getWidth() const;

	/**
	 * Save the image to a new file.
	 *
	 * @param fileName the name of the file to save to.
	 */
	void save(const std::string& fileName);

	/**
	 * Set a pixel of the Image.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 * @param pixel the color of the pixel.
	 */
	void setPixel(int x, int y, Color pixel);

	/**
	 * Write text onto the image using the default font.
	 *
	 * @param x the left coordinate.
	 * @param y the top coordinate.
	 * @param color the color of the text.
	 * @param text the string of text.
	 */
	void writeText( int x, int y, Color color, const std::string& text );

private:
	Color* pixels;
	int width;
	int height;
};

