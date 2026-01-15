#include "Player.h"

Player::Player() :
	x(750),
	y(500),
	rect({ x, y, RECT_WIDTH, RECT_HEIGHT }),
	color(PLAYER_IDLE_COLOR)
{
}

Player::~Player()
{
}

void Player::update(const bool* keys, float deltaTime, std::vector<SDL_Event>& events){
	float speed = 500.0f;
	float jump = 1000.0f;
	for(auto& pe : events) 
	{
		if (pe.type == SDL_EVENT_KEY_DOWN) {
			if (pe.key.key == SDLK_Q) {
				x -= speed * deltaTime;
				color = PLAYER_LEFT_COLOR;
			}
			if (pe.key.key == SDLK_D) {
				x += speed * deltaTime;
				color = PLAYER_RIGHT_COLOR;
			}
			if (pe.key.key == SDLK_Z){
				y -= jump * deltaTime;
				color = PLAYER_JUMP_COLOR;
			}
			if (pe.key.key == SDLK_S) {
				y += jump * deltaTime;
			}
		}
		if (pe.type == SDL_EVENT_KEY_UP) {
			color = PLAYER_IDLE_COLOR;
		}
	}
}

void Player::render(SDL_Renderer* renderer) {
	rect.x = x;
	rect.y = y;

	

	SDL_SetRenderDrawColorRGBA(renderer, color);
		
	
	
	SDL_RenderFillRect(renderer, &rect);
}
