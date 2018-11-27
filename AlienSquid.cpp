#include "AlienSquid.h"
#include "Alien.h"

AlienSquid::AlienSquid(size_t x, size_t y) {
	this->x = x;
	this->y = y;
	this->height = 8;
	this->width = 8;
	this->animation.frames = this->sprites;
	this->sprites[0] = new Sprite(8, 8);
	this->sprites[1] = new Sprite(8, 8);

	this->sprites[0]->setData(new uint8_t[64]{
		0, 0, 0, 1, 1, 0, 0, 0, // ...@@...
		0, 0, 1, 1, 1, 1, 0, 0, // ..@@@@..
		0, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@.
		1, 1, 0, 1, 1, 0, 1, 1, // @@.@@.@@
		1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@
		0, 1, 0, 1, 1, 0, 1, 0, // .@.@@.@.
		1, 0, 0, 0, 0, 0, 0, 1, // @......@
		0, 1, 0, 0, 0, 0, 1, 0  // .@....@.
	});

	this->sprites[1]->setData(new uint8_t[64]{
		0, 0, 0, 1, 1, 0, 0, 0,  // ...@@...
		0, 0, 1, 1, 1, 1, 0, 0,  // ..@@@@..
		0, 1, 1, 1, 1, 1, 1, 0,  // .@@@@@@.
		1, 1, 0, 1, 1, 0, 1, 1,  // @@.@@.@@
		1, 1, 1, 1, 1, 1, 1, 1,  // @@@@@@@@
		0, 0, 1, 0, 0, 1, 0, 0,  // ..@..@..
		0, 1, 0, 1, 1, 0, 1, 0,  // .@.@@.@.
		1, 0, 1, 0, 0, 1, 0, 1   // @.@..@.@
	});
}

int AlienSquid::getScore() {
	return 30;
}
