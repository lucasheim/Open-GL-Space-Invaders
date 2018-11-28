#pragma once
#include <stdint.h>
class Sprite
{
public:
	int width, height;
	uint8_t* data;
	Sprite();
	Sprite(int width, int height);
	void setData(uint8_t* data);
};

