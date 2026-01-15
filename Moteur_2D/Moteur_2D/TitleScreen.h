#pragma once
#include "Screen.h"
#include "ResourceManager.h"
#include <string>

class TitleScreen : public Screen
{
	SDL_Window* window;
	SDL_Renderer* Renderer;
	bool isFullScreen;

	ResourceManager& resources;
	std::string titleBackGroundId;

public:
	TitleScreen(SDL_Window* window, SDL_Renderer* renderer);

	inline bool isInFullScreen() const { return isFullScreen; }

	virtual Result update(Uint64 time, std::vector<SDL_Event>& e) override;
	virtual void renderer(SDL_Renderer* renderer) override;
};

