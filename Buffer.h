#pragma once
#include <stdint.h>
#include "Sprite.h"
class Buffer {
public:
	size_t width, height;
	uint32_t* data;
	Buffer(size_t width, size_t heigth);
	void colorClear(uint32_t color);
	void drawText(const Sprite& textSpritesheet, const char* text, size_t x, size_t y, uint32_t color);
	void drawSprite(const Sprite& sprite, size_t x, size_t y, uint32_t color);
	void drawNumber(const Sprite& numberSpritesheet, size_t number, size_t x, size_t y, uint32_t color);
};