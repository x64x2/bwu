/*
 * Text.hpp
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
 * @brief declares text rendering functions.
 */


#include <string>
#include "Font.hpp"

#ifndef TEXT_HPP
#define TEXT_HPP
/**
 * Draw bordered text.
 */
void drawBorderedText( const std::string& text );

/**
 * Draw bordered text scaled to the appropriate unit size defined by the game.
 */
void drawBorderedTextScaled( const std::string& text );

/**
 * Draw text.
 */
void drawText(const std::string& text);

/**
 * Draw text scaled to the appropriate unit size defined by the game.
 */
void drawTextScaled( const std::string& text );
