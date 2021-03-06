#include "Bullet.h"

Bullet::Bullet(int x, int y, int dir) {
	this->x = x;
	this->y = y;
	this->width = 1;
	this->height = 3;
	this->dir = dir;
}

Sprite* Bullet::getSprite() {
	Sprite* sprite = new Sprite(1, 3);
	sprite->setData(new uint8_t[1, 3]{
		1,	//@
		1,	//@
		1	//@
	});
	return sprite;
}

void Bullet::draw(Buffer* buffer) {
	buffer->drawSprite(this->getSprite(), this->x, this->y, this->color);
}

void Bullet::move() {
	this->y += this->dir;
}