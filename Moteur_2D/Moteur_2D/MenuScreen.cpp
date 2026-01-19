#include "MenuScreen.h"
#include <iostream>
MenuScreen::MenuScreen(SDL_Window* window, SDL_Renderer* renderer) :
	window(window)
	, Renderer(renderer)
	, resources(renderer)
	, backgroundId("menu_background")
	, buttonTextureId("button_texture")
	, fontId("menu_font")
	, titleFontId("title_font")
	, titleTextId("menu_title_text")
	, playButton(nullptr)
	, quitButton(nullptr)
	, lastWindowWidth(0)
	, lastWindowHeight(0)

{
	if (!this->resources.loadTexture(backgroundId, "assets/Background1.png"))
	{
		std::cerr << "[ERROR] MenuScreen failed to load assets/Background1.png\n";
	}

	if (!resources.loadTexture(buttonTextureId, "assets/button.png")) {
		std::cerr << "[WARNING] MenuScreen: no button texture, using colored rectangles\n";
	}

	if (!resources.loadFont(fontId, "assets/Font/ka1.ttf", 32)) {
		std::cerr << "[ERROR] MenuScreen failed to load font\n";
		return;
	}

	if (!resources.loadFont(titleFontId, "assets/Font/Fanzine Title.ttf", 144)) {
		std::cerr << "[ERROR] MenuScreen failed to load font\n";
		return;
	}

	SDL_Color white = { 255, 255, 255, 255 };
	if (!resources.createTextTexture(titleTextId, titleFontId, "SKYBOUND", white)) {
		std::cerr << "[ERROR] MenuScreen failed to create title text\n";
	}

	SetupButtons();

	std::cout << "[INFO] MenuScreen initialized successfully\n";
}

MenuScreen::~MenuScreen() {
	if (playButton) {
		delete playButton;
		playButton = nullptr;
	}
	if (quitButton) {
		delete quitButton;
		quitButton = nullptr;
	}
}

void MenuScreen::SetupButtons() {
	RecenterUI();
}

void MenuScreen::RecenterUI() {
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	float buttonWidth = 500;
	float buttonHeight = 120;
	float spacing = 80.0f;

	float centerX = (windowWidth - buttonWidth) / 2.0f;
	float titleY = windowHeight * 0.12;

	float buttonsStartY = windowHeight * 0.50f;
	float playY = buttonsStartY;
	float quitY = buttonsStartY + buttonHeight + spacing;

	std::cout << "[INFO] Recentered buttons - Window: " << windowWidth
		<< "x" << windowHeight << ", Play(" << centerX << "," << playY
		<< "), Quit(" << centerX << "," << quitY << ")\n";

	SDL_Texture* titleTexture = resources.getTexture(titleTextId);
	if (titleTexture) {
		float titleWidth = 0, titleHeight = 0;
		SDL_GetTextureSize(titleTexture, &titleWidth, &titleHeight);

		titleTextRect.w = titleWidth;
		titleTextRect.h = titleHeight;
		titleTextRect.x = (windowWidth - titleWidth) / 2.0f;
		titleTextRect.y = titleY;
	}

	SDL_Texture* btnTexture = resources.getTexture(buttonTextureId);

	if (playButton) {
		delete playButton;
	}
	playButton = new Button(btnTexture, centerX, playY, buttonWidth, buttonHeight);

	if (quitButton) {
		delete quitButton;
	}
	quitButton = new Button(btnTexture, centerX, quitY, buttonWidth, buttonHeight);

	TTF_Font* font = resources.getFont(fontId);
	if (font) {
		SDL_Color white = { 255, 255, 255, 255 };
		playButton->setText(Renderer, font, "Play", white);
		quitButton->setText(Renderer, font, "Quit", white);
	}
}

Screen::Result MenuScreen::update(Uint64 time, std::vector<SDL_Event>& events) {
	Result res = Screen::update(time, events);

	int currentWidth = 0;
	int currentHeight = 0;
	SDL_GetWindowSize(window, &currentWidth, &currentHeight);

	if (currentWidth != lastWindowWidth || currentHeight != lastWindowHeight) {
		std::cout << "[INFO] Window resized to " << currentWidth << "x" << currentHeight
			<< " - Recalculating button positions\n";
		RecenterUI();
		lastWindowWidth = currentWidth;
		lastWindowHeight = currentHeight;
	}

	for (auto& e : events)
	{
		if (playButton) playButton->handleEvent(e);
		if (quitButton) quitButton->handleEvent(e);
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
			if (playButton && playButton->isHovered()) {
				std::cout << "[INFO] Play button clicked - Starting game\n";
				return Result::nextScreen;
			}
			if (quitButton && quitButton->isHovered()) {
				std::cout << "[INFO] Quit button clicked - Exiting game\n";
				return Result::quitGame;
			}
		}
		if (e.type == SDL_EVENT_KEY_DOWN && (e.key.key == SDLK_ESCAPE)) {
			std::cout << "[INFO] ESC pressed - Exiting game\n";
			return Result::quitGame;
		}
	}
	return res;
}

void MenuScreen::renderer(SDL_Renderer* renderer) {
	SDL_Texture* bg = resources.getTexture(backgroundId);
	if (bg) {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);

		SDL_FRect dst;
		dst.x = 0.0f;
		dst.y = 0.0f;
		dst.w = static_cast<float>(w);
		dst.h = static_cast<float>(h);

		SDL_RenderTexture(renderer, bg, nullptr, &dst);
	}

	SDL_Texture* titleTexture = resources.getTexture(titleTextId);
	if (titleTexture) {
		SDL_RenderTexture(renderer, titleTexture, nullptr, &titleTextRect);
	}

	if (playButton) playButton->render(renderer);
	if (quitButton) quitButton->render(renderer);
}