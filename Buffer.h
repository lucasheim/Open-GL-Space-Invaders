#pragma once
#include <stdint.h>
#include "Sprite.h"
class Buffer {
public:
	int width, height;
	uint32_t* data;
	Buffer(int width, int heigth);
	void colorClear(uint32_t color);
	void drawText(const Sprite& textSpritesheet, const char* text, int x, int y, uint32_t color);
	void drawSprite(const Sprite* sprite, int x, int y, uint32_t color);
	void drawNumber(const Sprite& numberSpritesheet, int number, int x, int y, uint32_t color);
};