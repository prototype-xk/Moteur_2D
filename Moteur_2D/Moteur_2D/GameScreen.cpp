#include "GameScreen.h"
#include "Constant.h"
#include <iostream>
GameScreen::GameScreen(SDL_Window* window, SDL_Renderer* renderer) :
	window(window)
	, Renderer(renderer)
	, isFullScreen(false)
	, resources(Renderer)
	, titleBackGroundId("title_background")
	, playerSpawned(false)
	, screenWidth(0)
	, screenHeight(0)

{
	if (!this->resources.loadTexture(titleBackGroundId, "assets/Background2.jpg"))
	{
		std::cerr << "[ERROR] TitleScreen failed to load assets/Background2.jpg\n";
	}
}

Screen::Result GameScreen::update(Uint64 time, std::vector<SDL_Event>& events) {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	if (!playerSpawned || w != screenWidth || h != screenHeight) {
		screenWidth = w; screenHeight = h;
		player.respawn(static_cast<float>(w), static_cast<float>(h));  // Besoin implémenté
		playerSpawned = true;
		std::cout << "[PLAYER] Spawn x=" << player.x << " y=" << player.y << "\n";
	}

	Result res = Screen::update(time, events);
	for (auto& e : events) player.handleEvent(e);
	float deltaTime = time / 1000.0f;
	player.update(deltaTime);
	return res;
}

void GameScreen::renderer(SDL_Renderer* renderer) {
	SDL_Texture* bg = resources.getTexture(titleBackGroundId);
	if (!bg) {
		std::cerr << "[ERROR] TitleScreen background texture not found\n";
		return;
	}

	int w;
	int h;
	SDL_GetWindowSize(window, &w, &h);

	SDL_FRect dst;
	dst.x = 0.0f;
	dst.y = 0.0f;
	dst.w = static_cast<float>(w);
	dst.h = static_cast<float>(h);

	SDL_RenderTexture(renderer, bg, nullptr, &dst);
	player.render(renderer);
}