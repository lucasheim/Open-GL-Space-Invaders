#pragma once
#include "Sprite.h"
#include "IDrawable.h"
#include "Buffer.h"
#include "Formatter.h"
#include "GameConfig.h"

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
	int deathCounter = 10;
	bool dead = false;
	Sprite* sprites[2];
	SpriteAnimation animation = {
		2, // Frames quantity 
		ALIEN_ANIMATION_DURATION, 
		0, // Passed time
		NULL // Frames Array
	};
	uint32_t color = WHITE_UINT32;

	void draw(Buffer* buffer);
	void decreaseDeathCounter();
	Sprite* getNextSprite();
	Sprite* getDeathSprite();
};

