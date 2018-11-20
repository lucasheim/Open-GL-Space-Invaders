#pragma once
#include "Sprite.h"

struct SpriteAnimation
{
	size_t numFrames;
	size_t frameDuration;
	size_t time;
	Sprite** frames;
};

class Alien
{
public:
	size_t x, y, width, height;
	int deathCounter;
	bool dead;
	Sprite* sprites[2];
	SpriteAnimation animation;

	void decreaseDeathCounter();
	Sprite* getNextSprite();
};

