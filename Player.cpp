#include "Player.h"

Player::Player(int x, int y, int life) {
	this->x = x;
	this->y = y;
	this->life = life;
	this->width = 11;
	this->height = 7;
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

void Player::move(int movementDirection, int limit) {
	if (movementDirection != 0) {
		if (this->x + this->width + movementDirection >= limit) {
			this->x = limit - this->width;
		}
		else if ((int)this->x + movementDirection <= 0) {
			this->x = 0;
		}
		else {
			this->x += movementDirection;
		}
	}
}