/*
 * ResourceManager.cpp
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



#include <cstdlib>
#include "Animation.hpp"
#include "Background.hpp"
#include "BitmapFont.hpp"
#include "Exception.hpp"
#include "Globals.hpp"
#include "Image.hpp"
#include "LevelTheme.hpp"
#include "Music.hpp"
#include "ResourceManager.hpp"
#include "Sound.hpp"
#include "Texture.hpp"

static std::string getResourceFileName( const std::string& relativeFileName )
{
	return std::string("resources/") + relativeFileName;
}

ResourceManager::ResourceManager() :
	isMainResourceManager(true),
	textureAtlas(nullptr),
	atlasImage(nullptr),
	atlasX(0),
	atlasY(0),
	atlasMaxHeight(0),
	parent(nullptr)
{
}

ResourceManager::ResourceManager( const ResourceManager& parent ) :
	isMainResourceManager(false),
	textureAtlas(nullptr),
	atlasImage(nullptr),
	atlasX(0),
	atlasY(0),
	atlasMaxHeight(0),
	parent(&parent)
{
}

ResourceManager::~ResourceManager()
{
	// No need to free anything unless we are the main resource manager
	if( !isMainResourceManager )
	{
		return;
	}

	// Free the texture atlas
	delete textureAtlas;

	// Free the texture atlas image if it is still loaded in memory
	delete atlasImage;

	// Free all other textures
	for( auto texture : textures )
	{
		delete texture;
	}

	// Free all resources
	LOG << "Freeing all resources... ";
	for( auto resource : resources )
	{
		switch( resource.second.type )
		{
			case RESOURCE_ANIMATION:
				delete resource.second.animation;
				break;
			case RESOURCE_BACKGROUND:
				delete resource.second.background;
				break;
			case RESOURCE_FONT:
				delete resource.second.font;
				break;
			case RESOURCE_LEVELTHEME:
				delete resource.second.levelTheme;
				break;
			case RESOURCE_MUSIC:
				delete resource.second.music;
				break;
			case RESOURCE_SOUND:
				delete resource.second.sound;
				break;
		}
	}

	// Free all resource groups
	for( auto group : groups )
	{
		delete group.second;
	}

	LOG << "Done.\n";
}

void ResourceManager::bindTextureAtlas() const
{
	if( textureAtlas != nullptr )
	{
		textureAtlas->bind();
	}
}

const Animation* ResourceManager::getAnimation( const std::string& name ) const
{
	const Resource* resource = getResource(name);
	if( resource == nullptr || resource->type != RESOURCE_ANIMATION )
	{
		return nullptr;
	}
	return resource->animation;
}

const Background* ResourceManager::getBackground( const std::string& name ) const
{
	const Resource* resource = getResource(name);
	if( resource == nullptr || resource->type != RESOURCE_BACKGROUND )
	{
		LOG << "Warning: requested background resource \"" << name << "\" not found!\n";
		return nullptr;
	}

	return resource->background;
}

const BitmapFont* ResourceManager::getFont( const std::string& name ) const
{
	const Resource* resource = getResource(name);
	if( resource == nullptr || resource->type != RESOURCE_FONT )
	{
		LOG << "Warning: requested font resource \"" << name << "\" not found!\n";
		return nullptr;
	}

	return resource->font;
}

const LevelTheme* ResourceManager::getLevelTheme( const std::string& name ) const
{
	const Resource* resource = getResource(name);
	if( resource == nullptr || resource->type != RESOURCE_LEVELTHEME )
	{
		return nullptr;
	}

	return resource->levelTheme;
}

const std::vector<LevelTheme*>& ResourceManager::getLevelThemes() const
{
	return levelThemes;
}

int ResourceManager::getLoadingProgress() const
{
	return resources.size();
}

const Music* ResourceManager::getMusic( const std::string& name ) const
{
	const Resource* resource = getResource(name);
	if( resource == nullptr || resource->type != RESOURCE_MUSIC )
	{
		return nullptr;
	}

	return resource->music;
}

const ResourceManager::Resource* ResourceManager::getResource( const std::string& name ) const
{
	auto it = resources.find(name);
	if( it == resources.end() && parent == nullptr )
	{
		return nullptr;
	}
	else if( parent != nullptr )
	{
		return parent->getResource( name );
	}
	else
	{
		return &((*it).second);
	}
}

const ResourceManager* ResourceManager::getResourceGroup( const std::string& name ) const
{
	auto it = groups.find(name);
	if( it == groups.end() )
	{
		LOG << "Warning: resource group \"" << name << "\" not found!\n";
		return nullptr;
	}

	return (*it).second;
}

const Sound* ResourceManager::getSound( const std::string& name ) const
{
	const Resource* resource = getResource(name);
	if( resource == nullptr || resource->type != RESOURCE_SOUND )
	{
		return nullptr;
	}

	return resource->sound;
}

void ResourceManager::loadAnimations( xml_node<>* root )
{
	// Create an image to serve as a texture atlas if one hasn't been created already
	///@todo move this constant elsewhere
	static const int ATLAS_SIZE = 2048; // Storage size for 16k 16x16 tiles. Change if this ever gets too small
	if( atlasImage == nullptr )
	{
		atlasImage = new Image(ATLAS_SIZE, ATLAS_SIZE);

		atlasX = 0;
		atlasY = 0;
		atlasMaxHeight = 0;
	}

	// Create a cache for images
	std::map< std::string, Image* > images;

	// Structure for frames with raw information (not atlas coordinates)
	struct Frame
	{
		int x, y, w, h, xo, yo, duration;
		Image* image;
		std::string imageFile;
		bool mirrorX, mirrorY;
	};

	// Cache for frame templates
	std::map< std::string, Frame > frames;

	// Cache for duplicate frames
	std::map< std::string, Animation::Frame > atlasFrames;

	// Enumerate animations
	LOG << "Loading animations...\n";
	for( xml_node<>* node = root->first_node("animation"); node != nullptr; node = node->next_sibling("animation") )
	{
		// Check that it has an id
		xml_attribute<>* idAttr = node->first_attribute("id");
		if( idAttr == nullptr )
		{
			LOG << "Warning: Ignoring an animation that did not have an \"id\" attribute.\n";
			continue;
		}

		// Check for duplicates
		auto it = resources.find(idAttr->value());
		if( it != resources.end() )
		{
			LOG << "Warning: duplicate resource \"" << idAttr->value() << "\" found. It will be ignored.\n";
			continue;
		}

		// Check for x/y orientation of the animation
		bool xOrientation = false;
		bool yOrientation = true;
		xml_attribute<>* orientationAttr = node->first_attribute("x_orientation");
		if( orientationAttr != nullptr && strcmp(orientationAttr->value(), "right") == 0 )
		{
			xOrientation = true;
		}
		orientationAttr = node->first_attribute("y_orientation");
		if( orientationAttr != nullptr && strcmp(orientationAttr->value(), "down") == 0 )
		{
			yOrientation = false;
		}

		// Create an Animation Resource
		Animation* animation = new Animation(xOrientation, yOrientation);
		Resource resource;
		resource.type = RESOURCE_ANIMATION;
		resource.animation = animation;

		// These are preserved between frames to allow quasi-inheritance of frame properties
		int x = 0;
		int y = 0;
		int w = 1;
		int h = 1;
		int xo = 0;
		int yo = 0;
		int duration = 1;
		Image* image = nullptr;
		std::string imageFile;
		bool mirrorX = false;
		bool mirrorY = false;

		// Load all frames for the animation
		for( xml_node<>* frame = node->first_node("frame"); frame != nullptr; frame = frame->next_sibling("frame") )
		{
			// Whether the frame with the given ID should be saved for other frames to inherit
			bool createTemplate = false;

			// Check if the frame has an id
			xml_attribute<>* attr = frame->first_attribute("id");
			if( attr != nullptr )
			{
				// Check if the ID already exists
				std::map< std::string, Frame >::iterator it = frames.find( attr->value() );
				if( it == frames.end() )
				{
					// Copy this frame when it is done being parsed later...
					///@todo this is unnecessary, remove this functionality
					createTemplate = true;
				}
				else
				{
					Frame& f = (*it).second;
					x = f.x;
					y = f.y;
					w = f.w;
					h = f.h;
					xo = f.xo;
					yo = f.yo;
					duration = f.duration;
					image = f.image;
					imageFile = f.imageFile;
					mirrorX = f.mirrorX;
					mirrorY = f.mirrorY;
				}
			}

			// Check if it references a frame that we already loaded from another animation
			attr = frame->first_attribute("animation");
			if( attr != nullptr )
			{
				std::string animationName = attr->value();
				const Animation* a = getAnimation(animationName);
				if( a == nullptr )
				{
					LOG << "Warning : animation frame for animation \"" << idAttr->value() << "\" had an invalid animation specified.\n";
					continue;
				}

				attr = frame->first_attribute("index");
				if( attr == nullptr )
				{
					LOG << "Warning: animation frame for animation \"" << idAttr->value() << "\" had an animation specified without an index.\n";
					continue;
				}

				// Read the index as hex
				int index = strtol( attr->value(), NULL, 16 );
				if( index < 0 || index >= a->getLength() )
				{
					LOG << "Warning: animation frame for animation \"" << idAttr->value() << "\" had a bad index specified.\n";
					continue;
				}

				// Allow the duration of the frame to be specified
				attr = frame->first_attribute("time");
				if( attr != nullptr )
				{
					duration = std::atoi(attr->value());
				}

				// Also allow the mirroring of the frame to be specified
				attr = frame->first_attribute("mirror_x");
				if( attr != nullptr )
				{
					mirrorX = (std::atoi(attr->value()) == 1);
				}
				attr = frame->first_attribute("mirror_y");
				if( attr != nullptr )
				{
					mirrorY = (std::atoi(attr->value()) == 1);
				}

				Animation::Frame f =  a->getFrameBySequence(index);
				if( mirrorX )
				{
					std::swap(f.left, f.right);
				}
				if( mirrorY )
				{
					std::swap(f.bottom, f.top);
				}
				animation->addFrame( f, duration );
				continue;
			}

			// Get the image for the frame
			attr = frame->first_attribute("image");
			if( attr != nullptr )
			{
				imageFile = getResourceFileName(attr->value());
				std::map< std::string, Image* >::iterator it = images.find(imageFile);
				if( it == images.end() )
				{
					// Load the image
					image = new Image(imageFile);
					images[imageFile] = image;
				}
				else
				{
					// We've already loaded it
					image = (*it).second;
				}
			}
			else if( image == nullptr )
			{
				// No image found for this frame
				LOG << "Warning: Frame without a specified image was detected for animation \""
					<< idAttr->value() << "\". The frame will be ignored.\n";
				continue;
			}

			// Get other attributes for the frame
			attr = frame->first_attribute("x");
			if( attr != nullptr )
			{
				x = std::atoi(attr->value());
			}
			attr = frame->first_attribute("y");
			if( attr != nullptr )
			{
				y = std::atoi(attr->value());
			}
			attr = frame->first_attribute("w");
			if( attr != nullptr )
			{
				w = std::atoi(attr->value());
			}
			attr = frame->first_attribute("h");
			if( attr != nullptr )
			{
				h = std::atoi(attr->value());
			}
			attr = frame->first_attribute("xo");
			if( attr != nullptr )
			{
				xo = std::atoi(attr->value());
			}
			attr = frame->first_attribute("yo");
			if( attr != nullptr )
			{
				yo = std::atoi(attr->value());
			}
			attr = frame->first_attribute("time");
			if( attr != nullptr )
			{
				duration = std::atoi(attr->value());
			}
			attr = frame->first_attribute("mirror_x");
			if( attr != nullptr )
			{
				mirrorX = (std::atoi(attr->value()) == 1);
			}
			attr = frame->first_attribute("mirror_y");
			if( attr != nullptr )
			{
				mirrorY = (std::atoi(attr->value()) == 1);
			}

			// Also check if we are loading a sheet of tiles to create the animation
			int xStride = -1;
			int yStride = -1;
			attr = frame->first_attribute("x_stride");
			if( attr != nullptr && !createTemplate )
			{
				xStride = std::atoi(attr->value());
			}
			attr = frame->first_attribute("y_stride");
			if( attr != nullptr && !createTemplate )
			{
				yStride = std::atoi(attr->value());
			}

			// Create a copy of this frame for later use as a template for other frames
			if( createTemplate )
			{
				Frame f;
				f.x = x;
				f.y = y;
				f.w = w;
				f.h = h;
				f.xo = xo;
				f.yo = yo;
				f.duration = duration;
				f.image = image;
				f.imageFile = imageFile;
				f.mirrorX = mirrorX;
				f.mirrorY = mirrorY;
				frames[ frame->first_attribute("id")->value() ] = f;
			}

			// Now, add the frame(s) specified by the tag to the atlas
			int xMin = x;
			int xMax = x + 1;
			int xInc = 1;
			int yMin = y;
			int yMax = y + 1;
			int yInc = 1;
			Image* indexImage = nullptr;
			if( xStride != -1 && yStride != -1 )
			{
				xMax = image->getWidth();
				xInc = w + xStride;
				yMax = image->getHeight();
				yInc = h + yStride;

				// Create an index image depicting the indices for each frame
				indexImage = new Image(image->getWidth() * 2, image->getHeight() * 2);
				for( int a = 0; a < image->getWidth(); a++ )
				{
					for( int b = 0; b < image->getHeight(); b++ )
					{
						Color pixel = image->getPixel(a, b);
						indexImage->setPixel(a * 2, b * 2, pixel);
						indexImage->setPixel(a * 2 + 1, b * 2, pixel);
						indexImage->setPixel(a * 2, b * 2 + 1, pixel);
						indexImage->setPixel(a * 2 + 1, b * 2 + 1, pixel);
					}
				}
			}
			int frameNumber = 0;
			std::string frameString;
			bool addToAtlas = true;
			for( y = yMin; y < yMax; y += yInc )
			{
				for( x = xMin; x < xMax; x += xInc )
				{
					Animation::Frame f;
					if( xStride == -1 || yStride == -1 )
					{
						// Only copy to the atlas if the frame isn't a duplicate of one that has already been loaded
						// Construct a unique string that identifies the file and pixels for the frame, and hash it
						std::stringstream frameId;
						frameId << imageFile << ':' << x << ':' << y << ':' << w << ':' << h;
						frameString = frameId.str();
						std::map< std::string, Animation::Frame >::iterator it = atlasFrames.find( frameString );
						if( it != atlasFrames.end() )
						{
							// We don't need to add it to the atlas; it's already been copied
							f = (*it).second;
							f.xOffset = (double)xo / (double)UNIT_SIZE;
							f.yOffset = (double)yo / (double)UNIT_SIZE * -1.0;
							addToAtlas = false;
						}
					}

					if( addToAtlas )
					{
						// Check if the frame is all blank pixels
						// If so, skip it since it is a waste to store it
						bool skipFrame = true;
						for( int a = x; a < x + w; a++ )
						{
							for( int b = y; b < y + h; b++ )
							{
								if( image->getPixel(a, b).a != 0 )
								{
									skipFrame = false;
								}
							}
						}
						if( skipFrame )
						{
							continue;
						}

						// Wrap around the atlas when a row is full
						if( atlasX + w >= ATLAS_SIZE )
						{
							atlasX = 0;
							atlasY += atlasMaxHeight;
							atlasMaxHeight = 0;
						}

						// Copy pixels from the image to the atlas
						for( int a = 0; a < w; ++a )
						{
							for( int b = 0; b < h; ++b )
							{
								///@todo add blit function for Image
								atlasImage->setPixel(atlasX + a, atlasY + b, image->getPixel(x + a, y + b));
							}
						}

						f.left = (double)atlasX / (double)ATLAS_SIZE + 1e-6;
						f.right = (double)(atlasX + w) / (double)ATLAS_SIZE - 1e-6;
						f.bottom = (double)(atlasY + h) / (double)ATLAS_SIZE - 1e-6;
						f.top = (double)atlasY / (double)ATLAS_SIZE + 1e-6;
						f.xOffset = (double)xo / (double)UNIT_SIZE;
						f.yOffset = (double)yo / (double)UNIT_SIZE * -1.0;
						f.width = (double)w / (double)UNIT_SIZE;
						f.height = (double)h / (double)UNIT_SIZE;
					}

					// Add the frame to the animation
					if( mirrorX )
					{
						std::swap(f.left, f.right);
					}
					if( mirrorY )
					{
						std::swap(f.bottom, f.top);
					}
					animation->addFrame(
						f.left,
						f.right,
						f.bottom,
						f.top,
						f.xOffset,
						f.yOffset,
						f.width,
						f.height,
						duration
					);

					if( xStride == -1 || yStride == -1 )
					{
						// Save the frame in case there are duplicate uses of it later
						atlasFrames[frameString] = f;
					}
					else
					{
						// Save the frame number in the index image
						char tempString[8];
						sprintf(tempString, "%x", frameNumber);
						indexImage->writeText(x * 2, y * 2, Color::WHITE, tempString);
					}

					// Advance along the atlas row
					if( addToAtlas )
					{
						atlasX += w;
						if( h > atlasMaxHeight )
						{
							atlasMaxHeight = h;
						}
					}

					frameNumber++;
				} // Loop through all frames to be added (x direction)
			} // Loop through all frames to be added (y direction)

			if( indexImage != nullptr )
			{
				if( SETTINGS.debugMode )
				{
					indexImage->save( std::string(idAttr->value()) + "_index.png" );
				}
				delete indexImage;
			}

			// Restore values for x and y for frames that will reuse them
			x = xMin;
			y = yMin;
		} // Loop through <frame> tags

		// Save the resource
		resources[idAttr->value()] = resource;

		LOG << "Loaded animation \"" << idAttr->value() << "\".\n";
	} // Enumerate animations

	// Free loaded images
	for( auto image : images )
	{
		delete image.second;
	}
}

