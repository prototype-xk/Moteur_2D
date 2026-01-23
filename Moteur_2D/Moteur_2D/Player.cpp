#include "Player.h"

#define ANIM_WIDTH   64.f
#define ANIM_HEIGHT  64.f

#define ANIM_FRAMES_IDLE 2
#define ANIM_FRAMES_WALK 9

#define ANIM_DELAY_IDLE 300
#define ANIM_DELAY_WALK 100

enum AnimRow {
	WALK = 8,
	IDLE = 22,
	JUMP = 26,
	RUN = 36,
};

Player::Player(float screenW, float screenH, ResourceManager& res)
	: resources(res),
	x(screenW * 0.5f - RECT_WIDTH * 0.5f),
	y(screenH * 0.75f),
	rect{ x, y, RECT_WIDTH, RECT_HEIGHT },
	color(PLAYER_IDLE_COLOR),
	state(CHSTATE_IDLE),
	dir(CHDIR_SOUTH),
	anims(nullptr),
	animStartTime(0),
	updateTime(0)
{
	std::cout << "[PLAYER] Spawned at x=" << x
		<< " y=" << y
		<< " screen " << screenW << "x" << screenH << ")\n";

	resources.loadTexture("player_sprite", "assets/character-spritesheet.png");
	anims = resources.getTexture("player_sprite");

	if (anims == nullptr) {
		std::cerr << "[PLAYER ERROR] Failed to load sprite texture!\n";
	}
}

void Player::respawn(float screenW, float screenH) {
	x = screenW * 0.5f - RECT_WIDTH * 0.5f;
	y = screenH * 0.75f;
	rect = { x, y, RECT_WIDTH, RECT_HEIGHT };

	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;

	std::cout << "[PLAYER] RESPAWN x=" << x << " y=" << y << "\n";
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
	State newState = state;
	Uint64 currentTime = SDL_GetTicks();
	const float WALK_SPEED = 300.0f;
	const float MAX_SPEED = 500.0f;
	float speed = 0.0f;
	float jump = 600.0f;

	switch (state) {
	case CHSTATE_IDLE:
		if (moveLeft || moveRight) {
			newState = CHSTATE_WALKING;
			speed = WALK_SPEED;
			dir = moveLeft ? CHDIR_WEST : CHDIR_EAST;
		}
		break;

	case CHSTATE_WALKING:
		if (!moveLeft && !moveRight) {
			newState = CHSTATE_IDLE;
			speed = 0.0f;
		}
		else {
			speed = WALK_SPEED;
			dir = moveLeft ? CHDIR_WEST : CHDIR_EAST;

			float finalSpeed = moveLeft ? -speed : speed;

			x += finalSpeed * deltaTime;
		}
		break;

	case CHSTATE_JUMPING:
		break;
	}

	if (moveUp) {
		y -= jump * deltaTime;
	}
	if (moveDown) {
		y += jump * deltaTime;
	}

	if (state != newState) {
		state = newState;
		animStartTime = currentTime;
	}

	updateTime = currentTime;
	rect.x = x;
	rect.y = y;
}

void Player::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
	float screenX = x - cameraX;
	float screenY = y - cameraY;

	SDL_FRect screenRect = {
		screenX, screenY,
		RECT_WIDTH, RECT_HEIGHT
	};
	
	if (anims == nullptr) {
		std::cerr << "[PLAYER ERROR] anims is nullptr in render!\n";
		return;
	}

	int screenW, screenH;
	SDL_GetRenderOutputSize(renderer, &screenW, &screenH);

	float centerX = screenW * 0.5f - ANIM_WIDTH * 0.5f;
	float centerY = screenH * 0.5f - ANIM_HEIGHT * 0.5f;

	SDL_FRect rcdAnim = { centerX, centerY, ANIM_WIDTH, ANIM_HEIGHT };

	SDL_FRect rcsAnim = { 0, 0, ANIM_WIDTH, ANIM_HEIGHT };

	int row, frame;

	switch (state) {
	case CHSTATE_WALKING:
		row = AnimRow::WALK;
		frame = GetAnimFrame(ANIM_DELAY_WALK, ANIM_FRAMES_WALK);
		break;

	case CHSTATE_JUMPING:
		row = AnimRow::JUMP;
		frame = 0;
		break;

	default:
		row = AnimRow::IDLE;
		frame = GetAnimFrame(ANIM_DELAY_IDLE, ANIM_FRAMES_IDLE);
		break;
	}

	rcsAnim.x = ANIM_WIDTH * frame;
	rcsAnim.y = ANIM_HEIGHT * (row + dir);

	SDL_RenderTexture(renderer, anims, &rcsAnim, &rcdAnim);
}
