#include "Alien.h"

void Alien::updateDeathCounter() {
	if (this->shot && this->deathCounter) {
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
	if (this->hasBeenShot()) return this->drawDeath(buffer);
	buffer->drawSprite(this->getNextSprite(), this->x, this->y, this->color);
	this->animation.time++;
	if (this->animation.time == this->animation.numFrames * this->animation.frameDuration) {
		this->animation.time = 0;
	}
}

void Alien::drawDeath(Buffer * buffer) {
	buffer->drawSprite(this->getDeathSprite(), this->x, this->y, this->color);
}

bool Alien::hasBeenShot() {
	return this->shot;
}

bool Alien::isDead() {
	return this->deathCounter == 0;
}
