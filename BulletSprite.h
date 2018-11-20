#pragma once
#include "Sprite.h"
class BulletSprite : public Sprite
{
public:
	BulletSprite() : Sprite(1, 3) {
		this->setData(new uint8_t[this->width * this->height]{
			1,	//@
			1,	//@
			1	//@
		});
	}
};

