#include "Parallax.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Parallax::Parallax(SDL_Renderer* renderer) : m_renderer(renderer) {}

Parallax::~Parallax(){
	for (auto& layer : m_layers) {
		SDL_DestroyTexture(layer.texture);
	}
}

bool Parallax::addLayer(const std::string& imagePath, float speedRatio){
	SDL_Texture* texture = IMG_LoadTexture(m_renderer, imagePath.c_str());
	if (!texture) {
		std::cerr << "[PARALLAX] Failed to load: " << imagePath << std::endl;
		return false;
	}

	float w, h;
	SDL_GetTextureSize(texture, &w, &h);

	m_layers.push_back({
		texture, speedRatio, 0.0f, 0.0f, w, h
	});
	return true;
}

void Parallax::update(float deltaTime, float playerX, float playerY){
	for (auto& layer : m_layers) {
		//Le parallax suit le joueur avec un ratio plus petit donc plus lent
		layer.offsetX = -playerX * layer.speedRatio;
		layer.offsetY = -playerY * layer.speedRatio * 0.5f;

		//Boucle infinie horizontale
		while (layer.offsetX > 0) layer.offsetX -= layer.width;
		while (layer.offsetX < -layer.width) layer.offsetX += layer.width;
	}
}

void Parallax::render(SDL_Renderer* renderer, float screenWidth, float screenHeight){
	/*Rendu de fond en noir pour masquer les bords
	SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
	SDL_Rect bgRect = { 0, 0, (int)screenWidth, (int)screenHeight };
	SDL_RenderFillRect(renderer, &bgRect); */
	
	for (const auto& layer : m_layers) {
		SDL_FRect srcRect = { 0, 0, layer.width, layer.height };

		//Rendu de 2 copies pour l'effet infini
		float x = layer.offsetX;
		while (x < screenWidth) {
			SDL_FRect destRect = {
				x, layer.offsetY,
				layer.width, screenHeight
			};
			SDL_RenderTexture(renderer, layer.texture, &srcRect, &destRect);
			x += layer.width;
		}

		//Copie suivante pour couvrir tout l'écran
		x = layer.offsetX - layer.width;
		while (x < screenWidth) {
			SDL_FRect destRect = {
				x, layer.offsetY,
				layer.width, screenHeight
			};
			SDL_RenderTexture(renderer, layer.texture, &srcRect, &destRect);
			x += layer.width;
		}
	}

}
