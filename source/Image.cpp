/*
 * image.cpp
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

#include "Exception.hpp"
#include "Font.hpp"
#include "Image.hpp"

Image::Image(int width, int height) :
	width(width),
	height(height)
{
	pixels = new Color[width * height];
	for( int i = 0; i < width * height; ++i )
	{
		Color& pixel = pixels[i];
		pixel = Color::BLACK;
		pixel.a = 0;
	}
}

Image::Image(const std::string& fileName)
{
	ILboolean success;
	ILuint texId = ilGenImage();
	ilBindImage(texId);

	success = ilLoadImage(fileName.c_str());
	if( !success )
	{
		throw Exception() << "Couldn't load image \"" << fileName << "\"!";
	}

	success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	if( !success )
	{
		throw Exception() << "Couldn't convert image \"" << fileName << "\" while loading!";
	}

	// Copy image data from DevIL
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	pixels = new Color[width * height];
	ilCopyPixels(0, 0, 0, width, height, 32, IL_RGBA, IL_UNSIGNED_BYTE, (void*)pixels);

	ilDeleteImage(texId);
}

Image::~Image()
{
	delete [] pixels;
}

void Image::blit( const Image& source, int x, int y )
{
	for( int a = 0; a < source.width; a++ )
	{
		for( int b = 0; b < source.height; b++ )
		{
			setPixel( x + a, y + b, source.getPixel(a, b) );
		}
	}
}

int Image::getHeight() const
{
	return height;
}

Color Image::getPixel(int x, int y) const
{
	if( x < 0 || x >= width || y < 0 || y >= height )
	{
		return Color::BLACK;
	}

	return pixels[y * width + x];
}

int Image::getWidth() const
{
	return width;
}

void Image::save(const std::string& fileName)
{
	ILuint texId = ilGenImage();
	ilBindImage(texId);

	ilTexImage(width, height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, (void*)pixels);
	iluFlipImage();

	ilSaveImage( fileName.c_str() );
	ilDeleteImage(texId);
}

void Image::setPixel(int x, int y, Color pixel)
{
	if( x < 0 || x >= width || y < 0 || y >= height )
	{
		return;
	}

	pixels[y * width + x] = pixel;
}

void Image::writeText( int x, int y, Color color, const std::string& text )
{
	int xo = x;
	int yo = y;
	for( size_t i = 0; i < text.size(); i++ )
	{
		for( int a = 0; a < FONT_WIDTH; a++ )
		{
			for( int b = 0; b < FONT_HEIGHT; b++ )
			{
				if( fontData[(int)text[i]][b][a] == 1 )
				{
					setPixel(xo + a, yo + b, color);
				}
			}
		}

		if( text[i] == '\n' )
		{
			xo = x;
			yo += FONT_HEIGHT + 1;
		}
		else
		{
			xo += FONT_WIDTH + 1;
		}
	}
}
