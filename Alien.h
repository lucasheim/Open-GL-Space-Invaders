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
	bool shot = false;
	Sprite* sprites[2];
	SpriteAnimation animation = {
		2, // Frames quantity 
		ALIEN_ANIMATION_DURATION, 
		0, // Passed time
		NULL // Frames Array
	};
	
	void draw(Buffer* buffer);
	void drawDeath(Buffer* buffer);
	void decreaseDeathCounter();
	Sprite* getNextSprite();
	Sprite* getDeathSprite();
	bool hasBeenShot();
	bool isDead();
};

