#pragma once
#include "Sprite.h"
class PlayerSprite : public Sprite
{
public:
	PlayerSprite(size_t width, size_t height) : Sprite(width, height) {
		this->setData(new uint8_t[77] {
			0,0,0,0,0,1,0,0,0,0,0, // .....@.....
			0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
			0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
			0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
			1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
			1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
			1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
		});
	}
};

