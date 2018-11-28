#pragma once
#include "Sprite.h"
#include "IDrawable.h"

class Bullet : public IDrawable{
public:
	int dir;
	Bullet(int x, int y, int dir);
	Sprite* getSprite();
	void draw(Buffer * buffer);
	void move();
};

