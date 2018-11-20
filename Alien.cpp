#include "Alien.h"

void Alien::decreaseDeathCounter() {
	this->deathCounter--;
}

Sprite* Alien::getNextSprite() {
	int currentFrame = this->animation.time / this->animation.frameDuration;
	return this->animation.frames[currentFrame];
}
