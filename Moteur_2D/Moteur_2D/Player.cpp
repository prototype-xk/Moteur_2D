#include "Player.h"

Player::Player() :
{
}


Player::Player(int width, int height) :
	x(width / 2.0f),
	y(height - 100),
	rect({ width / 2.0f, height - 100.0, 32, 32 }),
	screenWidth(width),
	screenHeight(height)
{
}

Player::~Player()
{
}

void Player::update(const bool* keys, float deltaTime)
{
}

void Player::render(SDL_Renderer* renderer)
{
}

bool Player::checkCollision(const SDL_FRect& other)
{
	return false;
}
