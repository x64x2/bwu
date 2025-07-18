/*
 * World.hpp
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



#include <limits>
#include <list>
#include <set>
#include <vector>
#include "Animation.hpp"
#include "Enums.hpp"
#include "Vector2.hpp"

#ifndef WORLD_HPP
#define WORLD_HPP

class Background;
class Entity;
class Episode;
class Level;
class Music;
class Player;
class Random;
class Sprite;
class Tile;

/**
 * The status type of the world.
 */
enum WorldStatusType
{
	WORLD_RUNNING,    /**< The world is active and running. */
	WORLD_LEVEL_ENDED /**< The player finished the level. */
};

/**
 * The status of the world.
 */
struct WorldStatus
{
	WorldStatusType statusType;
	union
	{
		int exit; /**< The exit that was taken if the level ended. */
	};
};

/**
 * The environment that the game takes place in.
 */
class World
{
public:
	/**
	 * Create a new World.
	 */
	World();

	~World();

	/**
	 * Add a sprite to the World.
	 *
	 * @param sprite the sprite.
	 */
	void addSprite(Sprite* sprite);

	/**
	 * Add a sprite to the World.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 * @param sprite the sprite.
	 * @todo deprecate this.
	 */
	void addSprite(double x, double y, Sprite* sprite);

	/**
	 * Destroy a sprite.
	 */
	void destroySprite(Sprite* sprite);

	/**
	 * Destroy a tile.
	 */
	void destroyTile( Tile* tile );

	/**
	 * End the current level as if the player finished it.
	 *
	 * @param exit the exit number that was taken.
	 */
	void endLevel( int exit = 0 );

	/**
	 * Freeze the current time.
	 *
	 * @todo deprecate.
	 */
	void freezeTime();

	/**
	 * Get the delta (change in time) between each frame.
	 */
	double getDelta() const;

	/**
	 * Get the current frame number.
	 */
	int getFrameNumber() const;

	/**
	 * Get the height of the world.
	 */
	int getHeight() const;

	/**
	 * Get the player that is using the world.
	 */
	Player* getPlayer();

	/**
	 * Get the random number generator.
	 */
	Random& getRandom();

	/**
	 * Get a set of all sprites located on a certain tile.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 */
	const std::set<Sprite*>* getSprites( int x, int y ) const;

	/**
	 * Get a set of all sprites in a bounding box.
	 *
	 * @param left the left coordinate.
	 * @param bottom the bottom coordinate.
	 * @param width the width of the box.
	 * @param height the height of the box.
	 */
	std::set<Sprite*> getSpritesInBox( double left, double bottom, double width, double height );

	/**
	 * Get the status of the world.
	 */
	WorldStatus getStatus() const;

	/**
	 * Get a Tile at a position in the World.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 */
	const Tile* getTile( int x, int y ) const;

	/**
	 * Get a set of all tiles in a bounding box.
	 *
	 * @param left the left coordinate.
	 * @param bottom the bottom coordinate.
	 * @param width the width of the box.
	 * @param height the height of the box.
	 */
	std::set<Tile*> getTilesInBox( double left, double bottom, double width, double height );

	/**
	 * Get the time that is left to complete the level.
	 */
	int getTime() const;

	/**
	 * Get the width of the world.
	 */
	int getWidth() const;

	/**
	 * Check if a point in the world is underwater.
	 */
	bool isUnderwater( double x, double y ) const;

	/**
	 * Play the background music for the current level.
	 */
	void playBackgroundMusic() const;

	/**
	 * Remove a sprite from the World without destroying it.
	 */
	void removeSprite(Sprite* sprite);

	/**
	 * Render the world.
	 *
	 * @param viewX the center x coordinate.
	 * @param viewY the center y coordinate.
	 * @param viewWidth the width to display.
	 * @param viewHeight the height to display.
	 */
	void render(double viewX, double viewY, double viewWidth, double viewHeight);

	/**
	 * Set the Background used during rendering.
	 *
	 * @param name the name of the resource.
	 */
	void setBackground( const std::string& name );

	/**
	 * Set the current level, and load it.
	 */
	void setLevel( int levelId );

	/**
	 * Set the player that is using the world.
	 * This is used in some enemy routines to target the player.
	 *
	 * @note this does not add the player to the world!
	 */
	void setPlayer( Player* player );

	/**
	 * Set a tile at a position in the World.
	 *
	 * @param x the x coordinate.
	 * @param y the y coordinate.
	 * @param layer the rendering layer to use.
	 * @todo make a version of this function without the x and y parameters.
	 */
	void setTile(int x, int y, Tile* tile);

	/**
	 * Set whether the time should be enabled (not paused).
	 */
	void setTimeEnabled( bool enabled );

	/**
	 * Update the world by one tick.
	 *
	 * @param dt the delta.
	 */
	void update(double dt);

private:
	static constexpr double EPSILON = 0.2;
	static constexpr double DOUBLE_EPSILON = std::numeric_limits<double>::epsilon();

	struct Cell
	{
		std::set<Sprite*> sprites;
		Tile* tile;
		Sprite* spawn; /**< The sprite spawned when the Cell is first rendered. */
		bool underwater; /**< Whether the cell is underwater or not. */

		Cell();
		~Cell();
	};

	const Background* background;
	const Music* backgroundMusic;
	std::vector<Cell> cells;
	std::set<Tile*> deadTiles;
	double delta;
	int frameNumber;
	int height;
	Player* player;
	Random* random;
	std::list<Sprite*> sprites;
	WorldStatus status;
	int time;
	bool timeFrozen;
	int width;

	void destroyDeadTiles();

	bool doSpriteCollisionXAxisTest( Sprite* sprite, const Vector2<double>& oldPosition, bool tileCollisionsEnabled, bool spriteCollisionsEnabled );
	bool doSpriteCollisionYAxisTest( Sprite* sprite, const Vector2<double>& oldPosition, bool tileCollisionsEnabled, bool spriteCollisionsEnabled );
