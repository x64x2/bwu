/*
 * Tile.hpp
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


#include "Entity.hpp"
#include "Enums.hpp"
#include <string.h>

#ifndef TILE_HPP
#define TILE_HPP


class Sprite;

/// tile collision behavior//

enum TileCollisionBehavior
{
	TILE_NONE,       /**< Does not cause collisions. */

	TILE_PLATFORM,   /**< One-way vertical platform. Collisions only happen from above. */
	TILE_SLOPE_DOWN, /**< Downward slope. */
	TILE_SLOPE_UP,   /**< Upward slope. */
	TILE_SOLID       /**< Solid on all sides. */
};

/**
 * Entities that don't move.
 */
class Tile : public Entity
{
	friend class World;
public:
	/**
	 * Create a new tile.
	 *
	 * @param type the collision behavior of the Tile.
	 * @param tilesetEnabled whether the Tile should be rendered based on surrounding tiles.
	 */
	Tile(TileCollisionBehavior type, bool tilesetEnabled = false);

	/**
	 * Get the collision type of the tile.
	 */
	TileCollisionBehavior getCollisionBehavior() const;

	/**
	 * Get the edge state of a tile.
	 *
	 * @param edge the edge to get the state of.
	 * @return true if the edge is solid.
	 */
	bool getEdgeState(Edge edge) const;

	double getHeight() const;

	/**
	 * Get the height as an integer.
	 */
	int getHeightInt() const;

	double getWidth() const;

	/**
	 * Get the tileset animation for a unit tile of the Tile.
	 *
	 * @param x the x offset.
	 * @param y the y offset.
	 */
	const Animation* getTilesetAnimation( int x, int y ) const;

	/**
	 * Get the width as an integer.
	 */
	int getWidthInt() const;

	double getX() const;

	/**
	 * Get the x coordinate as an integer.
	 */
	int getXInt() const;

	double getY() const;

	/**
	 * Get the y coordinate as an integer.
	 */
	int getYInt() const;

	/**
	 * Check if the tile behaves as a slope.
	 */
	bool isSlope() const;

	/**
	 * Set the height of the tile.
	 */
	void setHeight(int height);

	void setLayer( int layer );

	/**
	 * Set the width of the tile.
	 */
	void setWidth(int width);

protected:
	virtual void onCollision(Sprite& sprite, Edge edge) {}
	virtual void onInit() {}

private:
	int x;
	int y;
	int width;
	int height;
	TileCollisionBehavior collisionBehavior;
	bool tilesetEnabled;
};
