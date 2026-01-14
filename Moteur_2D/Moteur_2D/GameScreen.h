#pragma once
#include "Screen.h"
class GameScreen : public Screen
{
	SDL_Window* window;
	bool isFullScreen;
	SDL_Texture* bg;
public:
	GameScreen();

	inline bool isInFullScreen() const { return isFullScreen; }

	virtual Result update(Uint64 time, std::vector<SDL_Event>& e) override;
	virtual void renderer(SDL_Renderer* renderer) override;
};
