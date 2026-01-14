#include "Screen.h"

Screen::Result Screen::update(Uint64 time, std::vector<SDL_Event>& e)
{
	for (auto it = e.begin(); it != e.end(); ++it)
	{
		if (it->type == SDL_EVENT_QUIT) {
			return Result::quitGame;
		}
	}
	return Result::sameScreen;
}
