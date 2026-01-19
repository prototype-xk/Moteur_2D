#include "Player.h"



Player::Player()
	: x(750),
	y(500),
	rect{ x, y, RECT_WIDTH, RECT_HEIGHT },
	color(PLAYER_IDLE_COLOR)
{
}

void Player::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
		if (e.key.key == SDLK_Q) moveLeft = true;
		if (e.key.key == SDLK_D) moveRight = true;
		if (e.key.key == SDLK_Z) moveUp = true;
		if (e.key.key == SDLK_S) moveDown = true;
	}
	if (e.type == SDL_EVENT_KEY_UP) {
		if (e.key.key == SDLK_Q) moveLeft = false;
		if (e.key.key == SDLK_D) moveRight = false;
		if (e.key.key == SDLK_Z) moveUp = false;
		if (e.key.key == SDLK_S) moveDown = false;
	}
}

void Player::update(float deltaTime) {
	float speed = 300.0f;
	float jump = 500.0f;

	if (moveLeft) {
		x -= speed * deltaTime;
		color = PLAYER_LEFT_COLOR;
	}
	if (moveRight) {
		x += speed * deltaTime;
		color = PLAYER_RIGHT_COLOR;
	}
	if (moveUp) {
		y -= jump * deltaTime;
		color = PLAYER_JUMP_COLOR;
	}
	if (moveDown) {
		y += jump * deltaTime;
	}
	if (!moveLeft && !moveRight && !moveUp && !moveDown) {
		color = PLAYER_IDLE_COLOR;
	}

	rect.x = x;
	rect.y = y;

}

void Player::render(SDL_Renderer* renderer) {
	rect.x = x;
	rect.y = y;



	SDL_SetRenderDrawColorRGBA(renderer, color);



	SDL_RenderFillRect(renderer, &rect);
}