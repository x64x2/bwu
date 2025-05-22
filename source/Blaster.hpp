#include "Tile.hpp"
#ifndef BLASTER_HPP
#define BLASTER_HPP


/**
 * A launcher for Bullet objects.
 */
class Blaster : public Tile
{
public:
	Blaster();

private:
	int lastBlastFrame;

	void onInit();
	void onRender();
};
