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
	float x, y;
	Uint32 color;
	SDL_FRect rect;

	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;

	Player();
	void handleEvent(const SDL_Event& e);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer);
	bool checkCollision(const SDL_FRect& other);
};

