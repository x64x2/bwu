/*
 * Rendering.hpp
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
 * @brief defines utility functions used during rendering.
 */
#ifndef RENDERING_HPP
#define RENDERING_HPP

/**
 * Clamp view coordinates so that nothing is rendered out of bounds.
 *
 * @param viewX the center x coordinate.
 * @param viewY the center y coordinate.
 * @param viewWidth the width of the viewport.
 * @param viewHeight the height of the viewport.
 * @param width the width of the area being rendered, in units.
 * @param height the height of the area being rendered, in units.
 */
void renderClampView( double& viewX, double& viewY, double viewWidth, double viewHeight, int width, int height );

/**
 * Clear the screen and set appropriate OpenGL settings.
 *
 * @param r the red component of the clear color.
 * @param g the green component of the clear color.
 * @param b the blue component of the clear color.
 * @param a the alpha component of the clear color.
 */
void renderClearScreen( float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.5f);

/**
 * Get the height of the screen in pixels.
 */
int renderGetScreenHeight();

/**
 * Get the width of the screen in pixels.
 */
int renderGetScreenWidth();

/**
 * Set the matrix units to represent individual pixels on the screen.
 */
void renderSetUnitsToPixels();

/**
 * Swap screen buffers.
 */
void renderSwapBuffers();
