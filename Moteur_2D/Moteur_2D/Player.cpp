#include "Player.h"

Player::Player() :
	x(750),
	y(200),
	rect({ x, y, RECT_WIDTH, RECT_HEIGHT })
{
}

Player::~Player()
{
}

void Player::update(const bool* keys, float deltaTime){
	float speed = 500;
	float jump = 200;
	while (SDL_PollEvent(&pe)) {
		if (pe.type == SDL_EVENT_KEY_DOWN) {
			if (pe.key.key == SDLK_Q) {
				x -= speed * deltaTime;
			}
			if (pe.type == SDLK_D) {
				x += speed * deltaTime;
			}
			if (pe.type == SDLK_SPACE){
				y += jump * deltaTime;
				y -= jump * deltaTime;
			}
		}
	}
}

void Player::render(SDL_Renderer* renderer) {
	if (pe.type == SDL_EVENT_KEY_DOWN) {
		if (pe.key.key == SDLK_Q) {
			SDL_SetRenderDrawColorRGBA(renderer, PLAYER_LEFT_COLOR);
		}
		if (pe.key.key == SDLK_D) {
			SDL_SetRenderDrawColorRGBA(renderer, PLAYER_RIGHT_COLOR);
		}
		if (pe.key.key == SDLK_SPACE) {
			SDL_SetRenderDrawColorRGBA(renderer, PLAYER_JUMP_COLOR);
		}
	}
	else {
		SDL_SetRenderDrawColorRGBA(renderer, PLAYER_STATIC_COLOR);
	}
}
