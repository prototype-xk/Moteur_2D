#pragma once
#include "Screen.h"
#include "ResourceManager.h"
#include "Button.h"
#include <string>
class MenuScreen : public Screen
{
	SDL_Window* window;
	SDL_Renderer* Renderer;
	bool isFullScreen;

	ResourceManager resources;
	std::string backgroundId;
	std::string buttonTextureId;
	std::string fontId;
	std::string titleFontId;
	std::string titleTextId;
	SDL_FRect titleTextRect;

	Button* playButton;
	Button* quitButton;

	int lastWindowWidth;
	int lastWindowHeight;

public:
	MenuScreen(SDL_Window* window, SDL_Renderer* renderer);
	~MenuScreen();

	inline bool isInFullScreen() const { return isFullScreen; }

	virtual Result update(Uint64 time, std::vector<SDL_Event>& e) override;
	virtual void renderer(SDL_Renderer* renderer) override;

private:
	void SetupButtons();
	void RecenterUI();
};

