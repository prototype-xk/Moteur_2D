#include "Parallax.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Parallax::Parallax(SDL_Renderer* renderer) : m_renderer(renderer) {}

Parallax::~Parallax(){
	for (auto& layer : m_layers) {
		SDL_DestroyTexture(layer.texture);
	}
}

bool Parallax::addLayer(const std::string& imagePath, float speedRatio, float scale, float initialX, float initialY){
	SDL_Texture* texture = IMG_LoadTexture(m_renderer, imagePath.c_str());
	if (!texture) {
		std::cerr << "[PARALLAX] Failed to load: " << imagePath << std::endl;
		return false;
	}

	float w, h;
	SDL_GetTextureSize(texture, &w, &h);

	m_layers.push_back({
		texture, speedRatio, 0.0f, 0.0f,
		w * scale, //Taille reduite
		h * scale, //Taille reduite
		scale, initialX, initialY // Decalages initiaux 
	});
	return true;
}

void Parallax::update(float deltaTime, float cameraX, float cameraY){
	for (auto& layer : m_layers) {
		//Le parallax suit le joueur avec un ratio plus petit donc plus lent
		layer.offsetX = -cameraX * layer.speedRatio + layer.initialOffsetX;
		layer.offsetY = -cameraY * layer.speedRatio + layer.initialOffsetY;

		//Boucle infinie horizontale
		while (layer.offsetX > 0) layer.offsetX -= layer.width;
		while (layer.offsetX < -layer.width) layer.offsetX += layer.width;

		//Boucle infini verticale
		while (layer.offsetY > 0) layer.offsetY -= layer.height;
		while (layer.offsetY < -layer.height) layer.offsetY += layer.height;
	}
}

void Parallax::render(SDL_Renderer* renderer, float screenWidth, float screenHeight){
	
	for (const auto& layer : m_layers) {
		SDL_FRect srcRect = { 0, 0, layer.width, layer.height };

		//Rendu horizontal infini
		float x = layer.offsetX;
		while (x < screenWidth) {
			SDL_FRect destRect = { x, layer.offsetY, layer.width, layer.height };
			SDL_RenderTexture(renderer, layer.texture, &srcRect, &destRect);
			x += layer.width;
		}

		//Copie suivante pour couvrir tout l'écran
		x = layer.offsetX - layer.width;
		while (x < screenWidth) {
			SDL_FRect destRect = { x, layer.offsetY, layer.width, screenHeight };
			SDL_RenderTexture(renderer, layer.texture, &srcRect, &destRect);
			x += layer.width;
		}
	}

}
