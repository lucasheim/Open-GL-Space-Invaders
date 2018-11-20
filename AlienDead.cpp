#include "AlienDead.h"

AlienDead::AlienDead(size_t x, size_t y) {
	this->x = x;
	this->y = y;
	this->height = 13;
	this->width = 7;
	this->sprites[0] = new Sprite(13, 7);

	this->sprites[0]->setData(new uint8_t[91]{
		0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
		0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
		0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
		1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
		0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
		0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
		0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
	});
}