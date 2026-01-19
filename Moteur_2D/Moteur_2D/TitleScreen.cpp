#include "TitleScreen.h"
#include <iostream>
TitleScreen::TitleScreen(SDL_Window* window, SDL_Renderer* renderer) :
	window(window)
	, Renderer(renderer)
	, isFullScreen(false)
	, resources(renderer)
	, titleBackGroundId("title_background")
	, fontId("title_font")
	, textTextureId("title_text")
	, textRect{ 0, 0, 0, 0 }

{
	if (!this->resources.loadTexture(titleBackGroundId, "assets/ScreenTitle.png"))
	{
		std::cerr << "[ERROR] TitleScreen failed to load assets/ScreenTitle.png\n";
	}

	if (!this->resources.loadFont(fontId, "assets/Font/000webfont.ttf", 48)) {
		std::cerr << "[ERROR] TitleScreen failed to load font\n";
		return;
	}

	SDL_Color white = { 255, 255, 255, 255 };
	if (!resources.createTextTexture(textTextureId, fontId, "Press ENTER to Start", white)) {
		std::cerr << "[ERROR] TitleScreen failed to create text texture\n";
		return;
	}

	SDL_Texture* textTexture = resources.getTexture(textTextureId);
	if (textTexture) {
		float textWidth = 0, textHeight = 0;
		if (SDL_GetTextureSize(textTexture, &textWidth, &textHeight)) {
			int windowWidth = 0, windowHeight = 0;
			SDL_GetWindowSize(window, &windowWidth, &windowHeight);

			textRect.w = textWidth;
			textRect.h = textHeight;
			textRect.x = (windowWidth - textWidth) / 2.0f;
			textRect.y = windowHeight * 0.75f;
		}
	}

	std::cout << "[INFO] TitleScreen initialized successfully\n";
}

Screen::Result TitleScreen::update(Uint64 time, std::vector<SDL_Event> &events) {
	Result res = Screen::update(time, events);

	for(const auto& e : events)
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

void TitleScreen::renderer(SDL_Renderer* renderer){
	SDL_Texture* bg = resources.getTexture(titleBackGroundId);
	if (bg) {
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

	SDL_Texture* textTexture = resources.getTexture(textTextureId);
	if (textTexture) {
		SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
	}

	return;
}