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
	SDL_FRect rect;
	SDL_Event pe;
	void update(const bool* keys, float deltaTime);
	void render(SDL_Renderer* renderer);
	bool checkCollision(const SDL_FRect& other);
};

