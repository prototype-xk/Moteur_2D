#include "GameOver.h"
#include <iostream>
GameOver::GameOver(SDL_Window* window, SDL_Renderer* renderer) :
	window(window)
	, Renderer(renderer)                 // <--- on stocke le renderer
	, isFullScreen(false)
	, resources(renderer)              // <--- on initialise la référence
	, titleBackGroundId("title_background")

{
	if (!this->resources.loadTexture(titleBackGroundId, "assets/ScreenTitle.png"))
	{
		std::cerr << "[ERROR] TitleScreen failed to load assets/ScreenTitle.png\n";
	}
}

Screen::Result GameOver::update(Uint64 time, std::vector<SDL_Event>& events) {
	Result res = Screen::update(time, events);

	for (auto& e : events)
	{
		if (e.type == SDL_EVENT_KEY_DOWN && (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER)) {
			isFullScreen = true;
			if (!SDL_SetWindowFullscreen(window, isFullScreen)) {
				std::cerr << "[ERROR] SDL_SetWindowFullscreen failed: " << SDL_GetError() << "\n";
				isFullScreen = false;
			}
			else {
				SDL_SyncWindow(window);
			}
			int w;
			int h;

			SDL_GetWindowSize(window, &w, &h);
			std::cout << "[INFO] Window size: " << w << "x" << h << "\n";
			return Result::nextScreen;
		}
	}
	return res;
}

void GameOver::renderer(SDL_Renderer* renderer) {
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
}