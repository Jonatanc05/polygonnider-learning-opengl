#include "Player.hpp"

Player::Player(float xP, float yP) {
	Player::xP = xP;
	Player::yP = yP;
	xV = yV = 0;
};

void Player::update() {
	xP += xV;
	yP += yV;
}
