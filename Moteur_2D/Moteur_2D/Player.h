#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>

class Player{
private:
	int screenWidth;
	int screenHeight;
public:
	Player();
	Player(int width, int height);
	~Player();
	float x, y;
	SDL_FRect rect;
	float width, height;
	void update(const bool* keys, float deltaTime);
	void render(SDL_Renderer* renderer);
	bool checkCollision(const SDL_FRect& other);
};

