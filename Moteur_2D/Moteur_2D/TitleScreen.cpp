#include "TitleScreen.h"
#include <iostream>
TitleScreen::TitleScreen(SDL_Window* window, SDL_Texture* bg) : window(window), isFullScreen(false), bg(bg)
{

}

Screen::Result TitleScreen::update(Uint64 time, std::vector<SDL_Event> &e) {
	Result res = Screen::update(time, e);

	for (auto it = e.begin(); res != Result::quitGame && it != e.end(); ++it)
	{
		if (it->type == SDL_EVENT_KEY_DOWN && (it->key.key == SDLK_RETURN || it->key.key == SDLK_KP_ENTER)) {
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

void TitleScreen::renderer(SDL_Renderer* renderer){

}