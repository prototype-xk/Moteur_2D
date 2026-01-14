#pragma once
#include <SDL3/SDL.h>
#include <vector>

class Screen
{
public:
	enum class Result
	{
		sameScreen,
		nextScreen,
		quitGame
	};

	virtual ~Screen() = default;
	virtual Result update(Uint64 time, std::vector<SDL_Event>& e);
	virtual void renderer(SDL_Renderer* renderer) = 0;
};

