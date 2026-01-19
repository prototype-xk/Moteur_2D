#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>

#include "TitleScreen.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "GameOver.h"
#include "ResourceManager.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum class GameScreenEnum {
	TitleScreen,
	MainMenu,
	Gameplay,
	GameOver
};

bool initSDL() {
	/*Initialisation de SDL*/
	std::cerr << "[INITIALIZATION] Starting SDL3 initialization\n";
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		std::cerr << "[DEBUG] SDL Init failed :" << SDL_GetError() << "\n";
		return false;
	}

	/*Initialisation de SDL TTF*/
	std::cerr << "[INITIALIZATION] Starting SDL3_ttf initialization\n";
	if (!TTF_Init()) {
		std::cerr << "[DEBUG] TTF Init failed :" << SDL_GetError() << "\n";
		SDL_Quit();
		return false;
	}
	return true;
}

bool createWindow(SDL_Window** window, SDL_Renderer** renderer) {
	/*Creation de la fenetre*/
	if (!SDL_CreateWindowAndRenderer("Skybound 0.1", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, window, renderer)) {
		std::cerr << "[ERROR] SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << "\n";
		return false;
	}
	/*Activation de VSync*/
	if (!SDL_SetRenderVSync(*renderer, 1)) {
		std::cerr << "[WARNING] VSync failed: " << SDL_GetError() << "\n";
	}

	/*Recup taille Fenetre*/
	int width = 0, height = 0;
	if (SDL_GetWindowSize(*window, &width, &height)) {
		std::cout << "[INFO] Window created: " << width << "x" << height << "\n";
	}
	
	return true;
}

int main () {
	if (!initSDL()) {
		return 1;
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	if (!createWindow(&window, &renderer)) {
		TTF_Quit();
		SDL_Quit();
	}

	bool running = true;
	GameScreenEnum currentScreen = GameScreenEnum::TitleScreen;
	Uint64 lastFrame = SDL_GetTicks();
	std::vector<SDL_Event> events;

	SDL_Event e;
	events.clear();

	TitleScreen titleScreen(window, renderer);
	MenuScreen menuScreen(window, renderer);
	GameScreen gameScreen(window, renderer);
	GameOver gameOverScreen(window, renderer);

	std::vector<Screen*> screens = {
		&titleScreen,
		&menuScreen,
		&gameScreen,
		&gameOverScreen
	};

	while (running) {
		Uint64 startFrame = SDL_GetTicks();
		Uint64 deltaTime = startFrame - lastFrame;
		lastFrame = startFrame;

		events.clear();
		while (SDL_PollEvent(&e)) {
			events.push_back(e);
		}

		Screen* activeScreen = screens[static_cast<int>(currentScreen)];
		Screen::Result result = activeScreen->update(deltaTime, events);

		switch (result) {
		case Screen::Result::quitGame:
			running = false;
			break;

		case Screen::Result::nextScreen:
			currentScreen = (currentScreen == GameScreenEnum::GameOver)
				? GameScreenEnum::MainMenu
				: static_cast<GameScreenEnum>(static_cast<int>(currentScreen) + 1);
			std::cout << "[INFO] Switched to screen: " << static_cast<int>(currentScreen) << "\n";
			break;

		case Screen::Result::sameScreen:
			break;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		activeScreen->renderer(renderer);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	std::cout << "[INFO] Game closed successfully\n";
}