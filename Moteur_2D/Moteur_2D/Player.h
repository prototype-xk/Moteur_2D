#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include "Color.h"
#include "ResourceManager.h"
#include "Constant.h"

#define RECT_WIDTH 32
#define RECT_HEIGHT 32

class Player {
public:
	Player() = default;
	Player(float screenW, float screenH, ResourceManager &res);
	void handleEvent(const SDL_Event& e);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer);
	bool checkCollision(const SDL_FRect& other);
	void respawn(float screenW, float screenH);
private:
	float x, y;
	Uint32 color;
	SDL_FRect rect;

	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;

	ResourceManager& resources;

	enum State {
		CHSTATE_IDLE,
		CHSTATE_WALKING,
		CHSTATE_JUMPING,
		CHSTATE_RUNNING
	} state;

	enum Dir {
		CHDIR_NORTH,
		CHDIR_WEST,
		CHDIR_SOUTH,
		CHDIR_EAST
	} dir;
};
