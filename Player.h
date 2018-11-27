#pragma once
#include "Sprite.h"
#include "IDrawable.h"
#include "Buffer.h"

class Player : public IDrawable {
public:
	size_t x, y;
	size_t life;
	Player(size_t x, size_t y, size_t life);
	Sprite* getSprite();
	void draw(Buffer* buffer);
};

