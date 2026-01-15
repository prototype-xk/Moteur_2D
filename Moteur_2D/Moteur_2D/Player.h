#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include "Color.h"

#define RECT_WIDTH 32
#define RECT_HEIGHT 32

class Player{
public:
	Player();
	~Player();
	float x, y;
	Uint32 color;
	SDL_FRect rect;
	SDL_Event pe;
	void update(const bool* keys, float deltaTime, std::vector<SDL_Event>& events);
	void render(SDL_Renderer* renderer);
	bool checkCollision(const SDL_FRect& other);
};

