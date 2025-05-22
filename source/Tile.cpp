/*
 * Tile.cpp
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
#include "Tile.hpp"
#include "World.hpp"

TileCollisionBehavior Tile::getCollisionBehavior() const
{
	return collisionBehavior;
}

bool Tile::getEdgeState(Edge edge) const
{
	switch( collisionBehavior )
	{
	case TILE_PLATFORM:
		return (edge == EDGE_TOP);

	case TILE_SLOPE_DOWN:
		return (edge == EDGE_LEFT);

	case TILE_SLOPE_UP:
		return (edge == EDGE_RIGHT);

	case TILE_SOLID:
		return true;

	default:
		break;
	}

	return false;
}

double Tile::getHeight() const
{
	return height;
}

int Tile::getHeightInt() const
{
	return height;
}

const Animation* Tile::getTilesetAnimation( int x, int y ) const
{
	const Animation* animation = nullptr;

	// Are we a slope?
	if( isSlope() )
	{
		if( width == 2 )
		{
			if( collisionBehavior == TILE_SLOPE_DOWN )
			{
				if( x == 0 )
				{
					animation = getAnimation( "tile_slope_down_left" );
				}
				else
				{
					animation = getAnimation( "tile_slope_down_right" );
				}
			}
			else
			{
				if( x == 0 )
				{
					animation = getAnimation( "tile_slope_up_left" );
				}
				else
				{
					animation = getAnimation( "tile_slope_up_right" );
				}
			}
		}
		else if( width == 1 )
		{
			if( collisionBehavior == TILE_SLOPE_DOWN )
			{
				animation = getAnimation( "tile_slope_down" );
			}
			else
			{
				animation = getAnimation( "tile_slope_up" );
			}
		}
	}
	// Are we a rectangular tile?
	else if( width > 1 || height > 1 )
	{
		if( x == 0 && width > 1 )
		{
			if( y == height - 1 )
			{
				animation = getAnimation( "tile_nw" );
				if( animation == nullptr )
				{
					animation = getAnimation( "tile_w" );
					if( animation == nullptr )
					{
						animation = getAnimation( "tile_n" );
					}
				}
			}
			else if( y == 0 )
			{
				animation = getAnimation( "tile_sw" );
				if( animation == nullptr )
				{
					animation = getAnimation( "tile_w" );
					if( animation == nullptr )
					{
						animation = getAnimation( "tile_s" );
					}
				}
			}
			else
			{
				animation = getAnimation( "tile_w" );
			}
		}
		else if( x == width - 1 && width > 1 )
		{
			if( y == height - 1 )
			{
				animation = getAnimation( "tile_ne" );
				if( animation == nullptr )
				{
					animation = getAnimation( "tile_e" );
					if( animation == nullptr )
					{
						animation = getAnimation( "tile_n" );
					}
				}
			}
			else if( y == 0 )
			{
				animation = getAnimation( "tile_se" );
				if( animation == nullptr )
				{
					animation = getAnimation( "tile_e" );
					if( animation == nullptr )
					{
						animation = getAnimation( "tile_s" );
					}
				}
			}
			else
			{
				animation = getAnimation( "tile_e" );
			}
		}
		else
		{
			if( y == height - 1 )
			{
				animation = getAnimation( "tile_n" );
			}
			else if( y == 0 )
			{
				animation = getAnimation( "tile_s" );
			}
		}
	}
	// Are we a level tile that is influenced by neighbors?
	else
	{
		auto resourceGroup = getResourceManager();

		auto westTile = getWorld().getTile(getXInt() + x - 1, getYInt() + y);
		auto eastTile = getWorld().getTile(getXInt() + x + 1, getYInt() + y);
		auto southTile = getWorld().getTile(getXInt() + x, getYInt() + y - 1);
		auto northTile = getWorld().getTile(getXInt() + x, getYInt() + y + 1);

		bool west = ((westTile && westTile->getResourceManager() == resourceGroup) || getXInt() + x == 0);
		bool east = ((eastTile && eastTile->getResourceManager() == resourceGroup) || getXInt() + x == getWorld().getWidth() - 1);
		bool south = ((southTile && southTile->getResourceManager() == resourceGroup) || getYInt() + y == 0);
		bool north = ((northTile && northTile->getResourceManager() == resourceGroup) || getYInt() + y == getWorld().getHeight() - 1);

		// Rectangular edges
		if( east && !north && !west && south )
		{
			animation = getAnimation( "tile_nw" );
		}
		else if( east && !north && west && south )
		{
			animation = getAnimation( "tile_n" );
		}
		else if( !east && !north && west && south )
		{
			animation = getAnimation( "tile_ne" );
		}
		else if( !east && north && west && south )
		{
			animation = getAnimation( "tile_e" );
		}
		else if( !east && north && west && !south )
		{
			animation = getAnimation( "tile_se" );
			if( animation == nullptr )
			{
				animation = getAnimation( "tile_e" );
			}
		}
		else if( east && north && west && !south )
		{
			animation = getAnimation( "tile_s" );
		}
		else if( east && north && !west && !south )
		{
			animation = getAnimation( "tile_sw" );
			if( animation == nullptr )
			{
				animation = getAnimation( "tile_w" );
			}
		}
		else if( east && north && !west && south )
		{
			animation = getAnimation( "tile_w" );
		}
		// Corners
		else if( east && north && west && south )
		{
			auto southWestTile = getWorld().getTile(getXInt() + x - 1, getYInt() + y - 1);
			auto southEastTile = getWorld().getTile(getXInt() + x + 1, getYInt() + y - 1);
			auto northWestTile = getWorld().getTile(getXInt() + x - 1, getYInt() + y + 1);
			auto northEastTile = getWorld().getTile(getXInt() + x + 1, getYInt() + y + 1);

			bool southWest = (southWestTile && southWestTile->getResourceManager() == resourceGroup);
			bool southEast = (southEastTile && southEastTile->getResourceManager() == resourceGroup);
			bool northWest = (northWestTile && northWestTile->getResourceManager() == resourceGroup);
			bool northEast = (northEastTile && northEastTile->getResourceManager() == resourceGroup);

			if( !northWest && northEast && southEast && southWest )
			{
				animation = getAnimation( "tile_corner_nw" );
			}
			else if( northWest && !northEast && southEast && southWest )
			{
				animation = getAnimation( "tile_corner_ne" );
			}
			else if( northWest && northEast && !southEast && southWest )
			{
				animation = getAnimation( "tile_corner_se" );
			}
			else if( northWest && northEast && southEast && !southWest )
			{
				animation = getAnimation( "tile_corner_sw" );
			}
		}
	}

	// If none of the rules worked, just return the default tile animation
	if( animation == nullptr )
	{
		animation = getAnimation( "tile" );
	}

	return animation;
}

