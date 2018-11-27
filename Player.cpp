#include "Player.h"

Player::Player(size_t x, size_t y, size_t life) {
	this->x = x;
	this->y = y;
	this->life = life;
}

Sprite* Player::getSprite() {
	Sprite* sprite = new Sprite(11, 7);
	sprite->setData(new uint8_t[77]{
			0,0,0,0,0,1,0,0,0,0,0, // .....@.....
			0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
			0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
			0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
			1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
			1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
			1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
	});
	return sprite;
}

void Player::draw(Buffer* buffer) {
	buffer->drawSprite(this->getSprite(), this->x, this->y, this->color);
}