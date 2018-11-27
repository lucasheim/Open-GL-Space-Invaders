#pragma once
#include "Sprite.h"
#include "IDrawable.h"

class Bullet : public IDrawable{
public:
	int dir;
	Bullet(size_t x, size_t y, int dir);
	Sprite* getSprite();
	void draw(Buffer * buffer);
	void move();
};

