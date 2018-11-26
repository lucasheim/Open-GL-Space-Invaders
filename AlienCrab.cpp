#include "AlienCrab.h"
#include "Alien.h"

AlienCrab::AlienCrab(size_t x, size_t y) {
	this->x = x;
	this->y = y;
	this->height = 11;
	this->width = 8;
	this->deathCounter = 10;
	this->dead = false;
	this->animation = {};
	this->animation.numFrames = 2;
	this->animation.frameDuration = 10;
	this->animation.frames = this->sprites;
	this->sprites[0] = new Sprite(11,  8);
	this->sprites[1] = new Sprite(11,  8);
	this->color = Formatter::rgbToUint32(255, 255, 255);

	this->sprites[0]->setData(new uint8_t[88]{
		0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,  // ..@.....@..
		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,  // ...@...@...
		0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,  // ..@@@@@@@..
		0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,  // .@@.@@@.@@.
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // @@@@@@@@@@@
		1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1,  // @.@@@@@@@.@
		1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,  // @.@.....@.@
		0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0  // ...@@.@@...
	});

	this->sprites[1]->setData(new uint8_t[88]{
		0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,  // ..@.....@..
		1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,  // @..@...@..@
		1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1,  // @.@@@@@@@.@
		1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,  // @@@.@@@.@@@
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // @@@@@@@@@@@
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // .@@@@@@@@@.
		0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,  // ..@.....@..
		0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0   // .@.......@.
	});
}

void AlienCrab::draw(Buffer* buffer) {
	buffer->drawSprite(this->getNextSprite(), this->x, this->y, this->color);
}