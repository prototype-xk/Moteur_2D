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

{
	m_parallax.addLayer("assets/Background1.png",1.0f, 1.0f); //Speed/ Taille normale

	m_parallax.addLayer("assets/Cloud1.png", 3.0f, 0.9, 0.0f, 200.0); //Speed/ 50% plus petit/ x / y

	m_parallax.addLayer("assets/Cloud1.png", 3.0f, 0.9, 800.0f, 100.0); //Speed/ 50% plus petit/ x / y

	m_parallax.addLayer("assets/Cloud1.png", 3.0f, 0.9, 400.0f, 500.0); //Speed/ 50% plus petit/ x / y

	
}

Screen::Result GameScreen::update(Uint64 time, std::vector<SDL_Event>& events) {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	if (!playerSpawned || w != screenWidth || h != screenHeight) {
		screenWidth = w; screenHeight = h;
		player.respawn(static_cast<float>(w), static_cast<float>(h));  // Besoin implémenté
		playerSpawned = true;
		std::cout << "[PLAYER] Spawn x=" << player.x << " y=" << player.y << "\n";
	}

	Result res = Screen::update(time, events);
	for (auto& e : events) player.handleEvent(e);
	float deltaTime = time / 1000.0f;
	player.update(deltaTime);
	//Camera suit le joueur
	m_camera.update(player.x, player.y);
	//Parallax update avec camera
	m_parallax.update(deltaTime, m_camera.getX(), m_camera.getY());

	return res;
}

void GameScreen::renderer(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColorRGBA(renderer, PARALLAX_NIGHT_SKY_COLOR);
	SDL_FRect bgRect = { 0, 0, screenWidth, screenHeight };
	SDL_RenderFillRect(renderer, &bgRect);

	m_parallax.render(renderer, screenWidth, screenHeight);

	// Joueur toujours centré à l'écran
	float playerScreenX = screenWidth * 0.5f - 50.0f;  // Centre horizontal
	float playerScreenY = screenHeight * 0.5f - 50.0f; // Centre vertical

	SDL_FRect playerScreenRect = {
		playerScreenX, playerScreenY,
		64.0f, 64.0f  // Taille joueur (ajustez)
	};

	
	player.render(renderer, m_camera.getX(), m_camera.getY());
}