#include "Buffer.h"

Buffer::Buffer(size_t width, size_t height) {
	this->width = width;
	this->height = height;
	this->data = new uint32_t[width * height];
}

void Buffer::colorClear(uint32_t color) {
	for (size_t i = 0; i < this->width * this->height; ++i)
	{
		this->data[i] = color;
	}
}

void Buffer::drawText(const Sprite& textSpritesheet, const char* text, size_t x, size_t y, uint32_t color) {
	size_t xp = x;
	size_t stride = textSpritesheet.width * textSpritesheet.height;
	Sprite sprite = textSpritesheet;
	for (const char* charp = text; *charp != '\0'; ++charp)
	{
		char character = *charp - 32;
		if (character < 0 || character >= 65) continue;
		sprite.data = textSpritesheet.data + character * stride;
		//this->drawSprite(sprite, xp, y, color);
		xp += sprite.width + 1;
	}
}

void Buffer::drawSprite(const Sprite* sprite, size_t startingX, size_t startingY, uint32_t color) {
	for (int x = 0; x < sprite->width; ++x) {
		for (int y = 0; y < sprite->height; ++y) {
			if (sprite->data[y * sprite->width + x] &&
				(sprite->height - 1 + startingY - y) < this->height &&
				(startingX + x) < this->width) {
				this->data[(sprite->height - 1 + startingY - y) * this->width + (startingX + x)] = color;
			}
		}
	}
}

void Buffer::drawNumber(const Sprite& numberSpritesheet, size_t number, size_t x, size_t y, uint32_t color) {
	uint8_t digits[64];
	size_t numDigits = 0;
	size_t currentNumber = number;
	do {
		digits[numDigits++] = currentNumber % 10;
		currentNumber = currentNumber / 10;
	} while (currentNumber > 0);
	size_t xp = x;
	size_t stride = numberSpritesheet.width * numberSpritesheet.height;
	Sprite sprite = numberSpritesheet;
	for (size_t i = 0; i < numDigits; ++i) {
		uint8_t digit = digits[numDigits - i - 1];
		sprite.data = numberSpritesheet.data + digit * stride;
		//this->drawSprite(sprite, xp, y, color);
		xp += sprite.width + 1;
	}
}
