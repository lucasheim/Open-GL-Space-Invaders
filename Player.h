#pragma once
#include "Sprite.h"
#include "IDrawable.h"
#include "Buffer.h"

class Player : public IDrawable {
public:
	int life;
	Player(int x, int y, int life);
	Sprite* getSprite();
	void draw(Buffer* buffer);
	void move(int movementDirection, int limit);
};

