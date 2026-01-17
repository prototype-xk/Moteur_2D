#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "Button.h"

#include "TitleScreen.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "GameOver.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum class GameScreenEnum {
	TitleScreen,
	MainMenu,
	Gameplay,
	GameOver
};

int main () {
	/*Initialisation de SDL*/
	std::cerr << "[INITIALIZATION] Starting SDL3 initialization\n";
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		std::cerr << "[DEBUG] SDL Init failed :" << SDL_GetError() << "\n";
		return 1;
	}

	/*Initialisation de SDL TTF*/
	std::cerr << "[INITIALIZATION] Starting SDL3_ttf initialization\n";
	if (!TTF_Init()) {
		std::cerr << "[DEBUG] TTF Init failed :" << SDL_GetError() << "\n";
		SDL_Quit();
		return 1;
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	/*Creation de la fenetre*/
	if (!SDL_CreateWindowAndRenderer("Skybound 0.1", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		std::cerr << "[ERROR] SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << "\n";
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	/*Activation de VSync*/
	if (!SDL_SetRenderVSync(renderer, 1)) {
		std::cerr << "[WARNING] VSync failed: " << SDL_GetError() << "\n";
	}

	/*Recup taille Fenetre*/
	int windowWidth = 0, windowHeight = 0;
	if (!SDL_GetWindowSize(window, &windowWidth, &windowHeight)) {
		std::cerr << "[ERROR] Failed to get window size: " << SDL_GetError() << "\n";
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	std::cout << "[INFO] Window size: " << windowWidth << "x" << windowHeight << "\n";

	int screenWidth = windowWidth;
	int screenHeight = windowHeight;
	std::cout << "[INFO] Game configured for resolution: " << screenWidth << "x" << screenHeight << "\n";

	SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "assets/Background1.png");
	if (!backgroundTexture) {
		std::cerr << "[ERROR] IMG_LoadTexture failed: " << SDL_GetError() << "\n";
	}

	SDL_Texture* titleBackgroundTexture = IMG_LoadTexture(renderer, "assets/ScreenTitle.png");
	if (!titleBackgroundTexture) {
		std::cerr << "[ERROR] IMG_LoadTexture title background failed : " << SDL_GetError() << "\n";
	}

	TTF_Font* font = TTF_OpenFont("assets/Font/000webfont.ttf", 36);
	if (!font) {
		std::cerr << "[ERROR] TTF_OpenFont failed: " << SDL_GetError() << "\n";
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	TTF_Font* fontPlay = TTF_OpenFont("assets/Font/ka1.ttf", 26);
	if (!fontPlay) {
		std::cerr << "[ERROR] TTF_OpenFont failed: " << SDL_GetError() << "\n";
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Color white = { 255,255,255,255 };

	SDL_Surface* titleTextSurface = TTF_RenderText_Blended(font, "Press Enter to Start", 0, white);
	if (!titleTextSurface) {
		std::cerr << "[ERROR] TTF_RenderText_Blended TitleScreen failed: " << SDL_GetError() << "\n";
	}

	SDL_Texture* titleTextTexture = nullptr;
	if (titleTextSurface) {
		titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);
		SDL_DestroySurface(titleTextSurface);
		if (!titleTextTexture) {
			std::cerr << "[ERROR] SDL_CreateTextureFromSurface TitleScreen failed: " << SDL_GetError() << "\n";
		}
	}

	SDL_FRect titleTextRect{};
	if (titleTextTexture) {
		float tw = 0, th = 0;
		if (!SDL_GetTextureSize(titleTextTexture, &tw, &th)) {
			std::cerr << "[ERROR] SDL_GetTextureSize titleTextTexture failed: " << SDL_GetError() << "\n";
		}
		titleTextRect.w = tw;
		titleTextRect.h = th;
		titleTextRect.x = (screenWidth - tw) / 2.0f;
		titleTextRect.y = (screenHeight * 0.75f);
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, "[DEBUG] Hello SDL3", 0, white);
	if (!textSurface) {
		std::cerr << "[ERROR] TTF_RenderText_Blended failed: " << SDL_GetError() << "\n";
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_DestroySurface(textSurface);

	if (!textTexture) {
		std::cerr << "[ERROR] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << "\n";
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	float textW = 0, textH = 0;
	if (!SDL_GetTextureSize(textTexture, &textW, &textH)) {
		std::cerr << "[ERROR] SDL_GetTextureSize failed: " << SDL_GetError() << "\n";
	}
	SDL_FRect textRect;
	textRect.x = 20.0f;
	textRect.y = 20.0f;
	textRect.w = textW;
	textRect.h = textH;
	SDL_Texture* buttonTexture = IMG_LoadTexture(renderer, "assets/button.png");
	if (!buttonTexture) {
		std::cerr << "[ERROR] IMG_LoadTexture button failed: " << SDL_GetError() << "\n";
	}
	Button buttonPlay(buttonTexture, 100.0f, 100.0f, 200.0f, 50.0f);

	SDL_Color black = { 0,0,0,255 };
	buttonPlay.setText(renderer, fontPlay, "Play", black);

	SDL_Cursor* defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
	SDL_Cursor* handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);

	if (defaultCursor) {
		SDL_SetCursor(defaultCursor);
	}

	bool running = true;
	bool isFullscreen = false;
	GameScreenEnum currentScreen = GameScreenEnum::TitleScreen;
	TitleScreen screen0(window, renderer);
	MenuScreen screen1(window, renderer);
	GameScreen screen2(window, renderer);
	GameOver screen3(window, renderer);

	std::vector<Screen> screens;
	SDL_Event e;

	screens.emplace_back(screen0);
	screens.emplace_back(screen1);
	screens.emplace_back(screen2);
	screens.emplace_back(screen3);
	Uint64 lastFrame = SDL_GetTicks();
	std::vector<SDL_Event> events;

	while (running) {
		Uint64 startFrame = SDL_GetTicks();
		Uint64 deltaTime = startFrame - lastFrame;

		events.clear();
		while (SDL_PollEvent(&e)) {
			events.push_back(e);
		}

		auto& currentScreenObj = screens[(int)currentScreen];

		switch (currentScreenObj.update(deltaTime, events))
		{
		case Screen::Result::quitGame:
			running = false;
			break;
		case Screen::Result::nextScreen:
			currentScreen = currentScreen == GameScreenEnum::GameOver
				? GameScreenEnum::MainMenu
				: (GameScreenEnum)((int)currentScreen + 1);
			break;
		case Screen::Result::sameScreen:
			// Nothing to do
			break;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {
			if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER) {
				currentScreen = GameScreenEnum::MainMenu;
				isFullscreen = true;
				if (!SDL_SetWindowFullscreen(window, isFullscreen)) {
					std::cerr << "[ERROR] SDL_SetWindowFullscreen failed: " << SDL_GetError() << "\n";
					isFullscreen = false;
				}
				else {
					SDL_SyncWindow(window);
				}
				SDL_GetWindowSize(window, &windowWidth, &windowHeight);
				std::cout << "[INFO] Window size: " << windowWidth << "x" << windowHeight << "\n";
			}
			if (e.key.key == SDLK_ESCAPE) {
				running = false;
			}
		}
			buttonPlay.handleEvent(e);

			if (currentScreen == GameScreenEnum::MainMenu &&
				e.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
				e.button.button == SDL_BUTTON_LEFT &&
				buttonPlay.isHovered()) {
				std::cout << "[SCREN] Switch MainMenu => GamePlay";
				currentScreen = GameScreenEnum::Gameplay;
			}

		if (buttonPlay.isHovered()) {
			if (handCursor) SDL_SetCursor(handCursor);
		}
		else {
			if (defaultCursor) SDL_SetCursor(defaultCursor);
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		switch (currentScreen) {
		case GameScreenEnum::TitleScreen:
			if (titleBackgroundTexture) {
				SDL_RenderTexture(renderer, titleBackgroundTexture, nullptr, nullptr);
			}

			if (titleTextTexture) {
				SDL_RenderTexture(renderer, titleTextTexture, nullptr, &titleTextRect);
			}
			break;
		case GameScreenEnum::MainMenu:
			SDL_RenderTexture(renderer, backgroundTexture, nullptr, nullptr);
			SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
			buttonPlay.render(renderer);
			break;

		case GameScreenEnum::Gameplay:
			SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
			SDL_RenderClear(renderer);
			break;
		}
		SDL_RenderPresent(renderer);
		lastFrame = startFrame;
	}
	if (handCursor)    SDL_DestroyCursor(handCursor);
	if (defaultCursor) SDL_DestroyCursor(defaultCursor);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(titleBackgroundTexture);
	TTF_CloseFont(fontPlay);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}