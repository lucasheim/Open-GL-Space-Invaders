#include "Sprite.h"

Sprite::Sprite(){}

Sprite::Sprite(size_t width, size_t height) {
	this->width = width;
	this->height = height;
	this->data = new uint8_t[width * height];
}

void Sprite::setData(uint8_t* data) {
	this->data = data;
}