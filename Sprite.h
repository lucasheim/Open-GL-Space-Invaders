#pragma once
#include <stdint.h>
class Sprite
{
public:
	size_t width, height;
	uint8_t* data;
	Sprite();
	Sprite(size_t width, size_t height);
	void setData(uint8_t* data);
};

