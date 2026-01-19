#pragma once
#include "Screen.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Camera.h"
#include "Parallax.h"
#include <string>
class GameScreen : public Screen
{
	SDL_Window* window;
	SDL_Renderer* Renderer;
	bool isFullScreen;

	ResourceManager resources;
	std::string titleBackGroundId;

	Camera m_camera;
	Parallax m_parallax;
	float m_worldWidth = 4000.0f;
	float m_worldHeight = 2000.0f;

public:
	GameScreen(SDL_Window* window, SDL_Renderer* renderer);

	inline bool isInFullScreen() const { return isFullScreen; }

	virtual Result update(Uint64 time, std::vector<SDL_Event>& e) override;
	virtual void renderer(SDL_Renderer* renderer) override;

private:
	Player player;
	std::vector<SDL_Event> eventsThisFrame;
	int screenWidth = 0;
	int screenHeight = 0;
	bool playerSpawned;
};