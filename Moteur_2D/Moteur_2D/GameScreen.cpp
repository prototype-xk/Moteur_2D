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
	m_parallax.addLayer("assets/Background2.jpg", 1.0f);
	m_parallax.addLayer("assets/Cloud1.png", 1.0f);
	m_parallax.addLayer("assets/Cloud1.png", 1.0f);

	// Récupère les dimensions et repositionne le joueur
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	screenWidth = w;
	screenHeight = h;
	player.respawn(static_cast<float>(w), static_cast<float>(h));  //  Utilise respawn() au lieu de =
	playerSpawned = true;
	std::cout << "[PLAYER] Initial spawn in GameScreen constructor\n";
}

Screen::Result GameScreen::update(Uint64 time, std::vector<SDL_Event>& events) {
	// Détecte le redimensionnement de la fenêtre
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

	//  VERSION SANS CLAMP : limite manuellement
	float safeX = player.GetX();
	float safeY = player.GetY();

	if (safeX < 0.0f) safeX = 0.0f;
	if (safeX > 10000.0f) safeX = 10000.0f;
	if (safeY < 0.0f) safeY = 0.0f;
	if (safeY > 10000.0f) safeY = 10000.0f;

	SDL_Log("Player brut: %.1f, %.1f | Safe: %.1f, %.1f",
		player.GetX(), player.GetY(), safeX, safeY);

	m_camera.update(safeX, safeY, m_worldWidth, m_worldHeight);
	m_parallax.update(deltaTime, safeX, safeY);

	return res;
}



void GameScreen::renderer(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColorRGBA(renderer, PARALLAX_NIGHT_SKY_COLOR);
	SDL_FRect bgRect = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
	SDL_RenderFillRect(renderer, &bgRect);

	m_parallax.render(renderer, screenWidth, screenHeight);

	player.render(renderer, m_camera.getX(), m_camera.getY());
}