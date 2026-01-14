#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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
	if (!SDL_CreateWindowAndRenderer("", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
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

	/*Initialisation Fonts*/
	TTF_Font* font = TTF_OpenFont("assets/000webfont.ttf", 36);
	if (!font) {
		std::cerr << "[ERROR] TTF_OpenFont failed: " << SDL_GetError() << "\n";
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Color white = { 255,255,255,255 };
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

	bool running = true;
	bool isFullscreen = false;
	SDL_Event e;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}

			if (e.type == SDL_EVENT_KEY_DOWN) {
				if (e.key.key == SDLK_F11) {
					isFullscreen = !isFullscreen;
					if (!SDL_SetWindowFullscreen(window, isFullscreen)) {
						std::cerr << "[ERROR] SDL_SetWindowFullscreen failed: " << SDL_GetError() << "\n";
						isFullscreen = !isFullscreen;
					}
					else {
						SDL_SyncWindow(window);
					}
					SDL_GetWindowSize(window, &windowWidth, &windowHeight);
					std::cout << "[INFO] Window size: " << windowWidth << "x" << windowHeight << "\n";
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, backgroundTexture, nullptr, nullptr);
		SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(backgroundTexture);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}