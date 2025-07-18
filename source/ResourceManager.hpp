/*
 * ResourceManager.hpp
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




#include <list>
#include <map>
#include <string>
#include <vector>

#define RESOURCEMANAGER_HPP

class Animation;
class Background;
class BitmapFont;
class Image;
struct LevelTheme;
class Music;
class Sound;
class Texture;

/**
 * Manages the loading and accessing of shared resources.
 */
class ResourceManager
{
public:
	/**
	 * Create a new resource manager.
	 */
	ResourceManager();

	~ResourceManager();

	/**
	 * Bind the texture atlas used for graphical resources.
	 */
	void bindTextureAtlas() const;

	/**
	 * Get an Animation resource.
	 *
	 * @param name the name of the resource.
	 */
	const Animation* getAnimation( const std::string& name ) const;

	/**
	 * Get a Background resource.
	 */
	const Background* getBackground( const std::string& name ) const;

	/**
	 * Get a font resource.
	 */
	const BitmapFont* getFont( const std::string& name ) const;

	/**
	 *  Get a LevelTheme resource.
	 */
	const LevelTheme* getLevelTheme( const std::string& name ) const;

	/**
	 * Get a container with all level themes.
	 */
	const std::vector<LevelTheme*>& getLevelThemes() const;

	/**
	 * Get the loading progress if resources are being loaded.
	 */
	int getLoadingProgress() const;

	/**
	 * Get a music track resource.
	 *
	 * @param name the name of the resource.
	 */
	const Music* getMusic( const std::string& name ) const;

	/**
	 * Get a resource group.
	 *
	 * @param name the name of the resource group.
	 */
	const ResourceManager* getResourceGroup( const std::string& name ) const;

	/**
	 * Load resources from a resource file.
	 *
	 * @param resourceFileName the name of the resource file.
	 */
	void loadResources( const std::string& resourceFileName );

	/**
	 * Play a music track.
	 *
	 * @param trackName the name of the music track.
	 * @param loop whether the track should loop infinitely or not.
	 */
	void playMusic( const std::string& trackName, bool loop ) const;

	/**
	 * Play a sound clip.
	 *
	 * @param soundName the name of the sound effect.
	 * @param channel the channel to play the effect on.
	 */
	void playSound( const std::string& soundName, int channel = -1 ) const;

private:
	enum ResourceType
	{
		RESOURCE_ANIMATION,
		RESOURCE_BACKGROUND,
		RESOURCE_FONT,
		RESOURCE_LEVELTHEME,
		RESOURCE_MUSIC,
		RESOURCE_SOUND
	};

	struct Resource
	{
		ResourceType type;

		union
		{
			Animation* animation;
			Background* background;
			BitmapFont* font;
			LevelTheme* levelTheme;
			Music* music;
			Sound* sound;
		};
	};

	bool isMainResourceManager;

	std::map< std::string, Resource > resources;

	Texture* textureAtlas;
	Image* atlasImage;
	int atlasX;
	int atlasY;
	int atlasMaxHeight;

	std::list<Texture*> textures;

	const ResourceManager* parent;
	std::map< std::string, ResourceManager* > groups;

	std::vector< LevelTheme* > levelThemes;

	/**
	 * Create a resource manager with a parent. Everything that the parent has
	 * will be shared with the child resource manager.
	 */
	ResourceManager( const ResourceManager& parent );

	const Resource* getResource( const std::string& name ) const;
	const Sound* getSound( const std::string& name ) const;
};
