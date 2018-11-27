#include "Alien.h"

void Alien::decreaseDeathCounter() {
	if (this->dead && this->deathCounter) {
		this->deathCounter--;
	}
}

Sprite* Alien::getNextSprite() {
	int currentFrame = this->animation.time / this->animation.frameDuration;
	return this->animation.frames[currentFrame];
}

Sprite* Alien::getDeathSprite() {
	Sprite* alienDeathSprite = new Sprite(13, 7);
	alienDeathSprite->setData(new uint8_t[91] {
		0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
		0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
		0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
		1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
		0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
		0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
		0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
	});
	return alienDeathSprite;
}

void Alien::draw(Buffer* buffer) {
	buffer->drawSprite(this->getNextSprite(), this->x, this->y, this->color);
	this->animation.time++;
	if (this->animation.time == this->animation.numFrames * this->animation.frameDuration) {
		this->animation.time = 0;
	}
}
