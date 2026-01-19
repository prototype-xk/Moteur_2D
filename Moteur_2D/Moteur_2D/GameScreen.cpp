#include "GameScreen.h"
#include "Constant.h"
#include <iostream>
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
	m_parallax.addLayer("assets/Background2.jpg", 0.3f);

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

	// Gestion des événements
	for (auto& e : events) {
		player.handleEvent(e);
	}

	// Calcul du deltaTime correct (en secondes)
	float deltaTime = (time - lastTime) / 1000.0f;
	lastTime = time;

	// Mise à jour du joueur
	player.update(deltaTime);

	// Camera suit le joueur (utilise les getters)
	m_camera.update(player.GetX(), player.GetY(), m_worldWidth, m_worldHeight);

	// Parallax update avec camera
	m_parallax.update(deltaTime, m_camera.getX(), m_camera.getY());

	return res;
}

void GameScreen::renderer(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColorRGBA(renderer, PARALLAX_NIGHT_SKY_COLOR);
	SDL_FRect bgRect = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
	SDL_RenderFillRect(renderer, &bgRect);

	m_parallax.render(renderer, screenWidth, screenHeight);

	// Le joueur gère maintenant son propre rendu avec les animations
	player.render(renderer);
}