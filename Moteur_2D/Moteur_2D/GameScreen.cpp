#include "GameScreen.h"
#include "Constant.h"
#include <iostream>
#include <algorithm>
GameScreen::GameScreen(SDL_Window* window, SDL_Renderer* renderer) :
	window(window)
	, Renderer(renderer)
	, isFullScreen(false)
	, resources(Renderer)
	, titleBackGroundId("title_background")
	, m_camera(SCREEN_WIDTH, SCREEN_HEIGHT)
	, m_parallax(renderer)
	, playerSpawned(false)
	, screenWidth(0)
	, screenHeight(0)
	, lastTime(SDL_GetTicks())
	, player(0.0f, 0.0f, resources)  // Initialisation du player
{
	m_parallax.addLayer("assets/Background2.jpg",1.0f, 4.5f); //Speed/ Taille normale

	m_parallax.addLayer("assets/Cloud1.png", 3.0f, 0.5, 0.0f, 200.0); //Speed/ 10% plus petit/ x / y

	m_parallax.addLayer("assets/Cloud1.png", 3.0f, 0.5, 800.0f, 100.0); //Speed/ 10% plus petit/ x / y

	m_parallax.addLayer("assets/Cloud1.png", 3.0f, 0.5, 400.0f, -1000.0); //Speed/ 10% plus petit/ x / y

	
}

Screen::Result GameScreen::update(Uint64 time, std::vector<SDL_Event>& events) {
	// D�tecte le redimensionnement de la fen�tre
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	if (w != screenWidth || h != screenHeight) {
		screenWidth = w;
		screenHeight = h;
		player.respawn(static_cast<float>(w), static_cast<float>(h));
		std::cout << "[PLAYER] Respawn due to window resize\n";
	}

	Result res = Screen::update(time, events);

	for (auto& e : events) {
		player.handleEvent(e);
	}

	float deltaTime = (time - lastTime) / 1000.0f;
	lastTime = time;

	player.update(deltaTime);
	//Camera suit le joueur
	m_camera.update(player.x, player.y);
	//Parallax update avec camera
	m_parallax.update(deltaTime, m_camera.getX(), m_camera.getY());

	return res;
}



void GameScreen::renderer(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColorRGBA(renderer, PARALLAX_NIGHT_SKY_COLOR);
	SDL_FRect bgRect = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
	SDL_RenderFillRect(renderer, &bgRect);

	m_parallax.render(renderer, screenWidth, screenHeight);

	// Joueur toujours centr� � l'�cran
	float playerScreenX = screenWidth * 0.5f - 50.0f;  // Centre horizontal
	float playerScreenY = screenHeight * 0.5f - 50.0f; // Centre vertical

	SDL_FRect playerScreenRect = {
		playerScreenX, playerScreenY,
		64.0f, 64.0f  // Taille joueur (ajustez)
	};

	
	player.render(renderer, m_camera.getX(), m_camera.getY());
}