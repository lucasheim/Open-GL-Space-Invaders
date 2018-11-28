#pragma once
#include "Sprite.h"
#include "IDrawable.h"
#include "Buffer.h"
#include "Formatter.h"
#include "GameConfig.h"

struct SpriteAnimation
{
	int numFrames;
	int frameDuration;
	int time;
	Sprite** frames;
};

class Alien : public IDrawable {
public:
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
	void updateDeathCounter();
	Sprite* getNextSprite();
	Sprite* getDeathSprite();
	bool hasBeenShot();
	bool isDead();
	virtual int getScore() = 0;
};

