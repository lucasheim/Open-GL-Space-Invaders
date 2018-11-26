#pragma once
#include "Sprite.h"
#include "IDrawable.h"

struct SpriteAnimation
{
	size_t numFrames;
	size_t frameDuration;
	size_t time;
	Sprite** frames;
};

class Alien : public IDrawable
{
public:
	size_t x, y, width, height;
	int deathCounter;
	bool dead;
	Sprite* sprites[2];
	SpriteAnimation animation;
	uint32_t color;

	virtual void draw(Buffer* buffer) = 0;
	void decreaseDeathCounter();
	Sprite* getNextSprite();
	Sprite* getDeathSprite();
};

