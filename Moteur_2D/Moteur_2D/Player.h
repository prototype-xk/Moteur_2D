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
	Player(float screenW, float screenH, ResourceManager& res);
	void handleEvent(const SDL_Event& e);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer, float cameraX, float cameraY);
	bool checkCollision(const SDL_FRect& other);
	void respawn(float screenW, float screenH);

	inline float GetX() const { return x; }
	inline float GetY() const { return y; }
private:
	ResourceManager& resources;

	float x;
	float y;
	Uint32 color;
	SDL_FRect rect;

	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;

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

	SDL_Texture* anims;
	Uint64 animStartTime;
	Uint64 updateTime;
	float speed;

	inline int GetAnimFrame(int delay, int frame_count) const {
		return ((int)(updateTime - animStartTime) / delay) % frame_count;
	}
};
