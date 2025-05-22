/*
 * HillyLevelGenerator.cpp
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


#include "HillyLevelGenerator.hpp"

#define LEVEL_WIDTH 256
#define LEVEL_HEIGHT 32

Level* HillyLevelGenerator::generateLevel( int seed ) const
{
	Random random;
	random.seed(seed);
	Level* level = new Level(LEVEL_WIDTH, LEVEL_HEIGHT);

	int platformLength = 1;
	int heights[LEVEL_WIDTH];
	heights[0] = 4;
	heights[1] = 4;
	heights[LEVEL_WIDTH - 2] = 4;
	heights[LEVEL_WIDTH - 1] = 4;

	// First pass - determine the height of the base terrain
	for( int x = 2; x < LEVEL_WIDTH - 2; x++ )
	{
		platformLength--;
		if( platformLength == 0 )
		{
			platformLength = random.nextInt() % 6 + 4;

			// Make a gap
			if( heights[x - 1] != 0 && platformLength < 6 && random.nextInt() % 2 == 0 )
			{
				heights[x] = 0;
			}
			else
			{
				heights[x] = heights[x - 1] + random.nextInt() % 9 - 5;

				// Clamp heights
				if( heights[x] < 2 )
				{
					heights[x] = 2;
				}
				else if( heights[x] > 12 )
				{
					heights[x] = 12;
				}
			}
			continue;
		}

		// Continue building a gap
		if( heights[x - 1] == 0 )
		{
			heights[x] = 0;
			continue;
		}

		if( random.nextInt() % 2 == 0 )
		{
			heights[x] = heights[x - 1] + random.nextInt() % 3 - 1;
		}
		else
		{
			heights[x] = heights[x - 1];
		}

		// Clamp heights
		if( heights[x] < 2 )
		{
			heights[x] = 2;
		}
		else if( heights[x] > 12 )
		{
			heights[x] = 12;
		}
	}

	// Second pass - build terrain
	for( int x = 2; x < LEVEL_WIDTH - 2; x++ )
	{
		// Remove single width columns
		if( heights[x - 1] > heights[x] && heights[x - 2] < heights[x - 1] )
		{
			heights[x] = heights[x - 1];
		}

		// Build the terrain
		for( int y = 0; y < heights[x]; y++ )
		{
			level->addTile(x, y, TYPE_GROUND);
		}
	}

	// Third pass - fill in holes
	for( int x = 2; x < LEVEL_WIDTH - 2; x++ )
	{
		if( heights[x - 2] == heights[x + 1] && heights[x - 1] == heights[x] && heights[x] == heights[x + 1] - 1 )
		{
			level->addTile(x, heights[x]++, TYPE_GROUND);
			level->addTile(x - 1, heights[x - 1]++, TYPE_GROUND);
		}
	}
	for( int x = 1; x < LEVEL_WIDTH - 1; x++ )
	{
		if( heights[x] < heights[x + 1] && heights[x] < heights[x - 1] )
		{
			level->addTile(x, heights[x]++, TYPE_GROUND);
		}
	}

	// Fourth pass - build slopes, blocks, and enemies
	for( int x = 4; x < LEVEL_WIDTH - 3; x++ )
	{
		if( heights[x] == heights[x - 1] && heights[x - 2] < heights[x] + 1 && heights[x + 1] == heights[x] + 1 && heights[x - 3] <= heights[x - 2] &&
			!(heights[x - 3] == heights[x - 2] && heights[x - 2] == heights[x - 1] && heights[x - 1] == heights[x] && heights[x - 4] == heights[x - 3] + 1 && heights[x + 1] == heights[x] + 1 ) ) // Prevents"V" shaped slopes
		{
			level->addTile(x - 1, heights[x], TYPE_SLOPE_UP, 2, 1);
		}
		else if( heights[x] == heights[x - 1] && heights[x + 1] < heights[x] + 1 && heights[x - 2] == heights[x] + 1 && heights[x + 2] <= heights[x + 1] &&
			!(heights[x - 1] == heights[x] && heights[x] == heights[x + 1] && heights[x + 1] == heights[x + 2] && heights[x - 2] == heights[x - 1] + 1 && heights[x + 3] == heights[x + 2] + 1) ) // Prevents "V" shaped slopes
		{
			level->addTile(x - 1, heights[x], TYPE_SLOPE_DOWN, 2, 1);
		}
		else if( heights[x] + 1 == heights[x + 1] && heights[x - 1] < heights[x] + 1 &&
			!(heights[x] == heights[x - 1] && heights[x - 2] == heights[x + 1] && heights[x] + 1 == heights[x + 1]) ) // Prevents "V" shaped slopes
		{
			level->addTile(x, heights[x], TYPE_SLOPE_UP);
		}
		else if( heights[x] + 1 == heights[x - 1] && heights[x + 1] < heights[x] + 1 &&
			!(heights[x] == heights[x + 1] && heights[x - 1] == heights[x + 2] && heights[x] + 1 == heights[x - 1]) ) // Prevents "V" shaped slopes
		{
			level->addTile(x, heights[x], TYPE_SLOPE_DOWN);
		}
		else if( random.nextInt() % 5 == 0 && heights[x] != 0 )
		{
			SpriteType enemy = TYPE_SPRITE_NULL;
			switch( random.nextInt() % 2 )
			{
				case 0:
					enemy = TYPE_GOOMBA;
					break;

				case 1:
					enemy = TYPE_KOOPA;
					break;
			}
			level->addSprite(x, heights[x], enemy);
		}

		if( heights[x] != 0 && random.nextInt() % 4 == 0 )
		{
			TileType blockType;
			int coins = (random.nextInt() % 5 - 2) * 5;
			if( random.nextInt() % 3 || coins <= 0 )
			{
				blockType = TYPE_BRICK;
			}
			else
			{
				blockType = TYPE_QUESTION_BLOCK;
			}
			int block = level->addTile(x, heights[x] + 4, blockType );
			if( coins <= 0 && blockType == TYPE_QUESTION_BLOCK )
			{
				std::string contents;
				switch( random.nextInt() % 3 )
				{
				case 0:
					contents = "mushroom";
					break;
				case 1:
					contents = "flower";
					break;
				case 2:
					contents = "leaf";
					break;
				}
				level->setTileData(block, "contents", contents);
			}
			else if( coins > 0 )
			{
				level->setTileData(block, "coins", coins );
				if( random.nextInt() % 4 == 0 )
				{
					level->setTileData(block, "scatter_coins", 1);
				}
			}
		}
	}

	// Start pipe and end pipe
	level->addTile(0, 0, TYPE_PIPE_UP, 2, 4);
	level->addTile(LEVEL_WIDTH - 2, 0, TYPE_PIPE_UP, 2, 4);

	// Level end
	level->addSprite(LEVEL_WIDTH - 2, 7, TYPE_LEVEL_END);

	return level;
}
