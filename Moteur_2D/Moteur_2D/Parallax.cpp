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
		texture,
		speedRatio,
		0.0f, 0.0f,
		w,
		h,
		scale,
		initialX,
		initialY 
	});
	return true;
}

void Parallax::update(float deltaTime, float cameraX, float cameraY) {
	for (auto& layer : m_layers) {
		//Le parallax suit le joueur avec un ratio plus petit donc plus lent
		layer.offsetX = -cameraX * layer.speedRatio + layer.initialOffsetX;
		layer.offsetY = -cameraY * layer.speedRatio + layer.initialOffsetY;

	}
}

void Parallax::render(SDL_Renderer* renderer, float screenWidth, float screenHeight) {

	for (const auto& layer : m_layers) {
		SDL_FRect srcRect = { 0, 0, layer.width, layer.height };

		float drawW = layer.width * layer.scale;
		float drawH = layer.height * layer.scale;

		// ? MODULO LOCAL pour le rendu seulement
		float renderOffsetX = fmodf(layer.offsetX, drawW);
		float renderOffsetY = fmodf(layer.offsetY, drawH);

		// Boucle infinie parfaite
		for (float x = renderOffsetX - drawW; x < screenWidth + drawW; x += drawW) {
			for (float y = renderOffsetY - drawH; y < screenHeight + drawH; y += drawH) {
				SDL_FRect destRect = { x, y, drawW, drawH };
				SDL_RenderTexture(renderer, layer.texture, &srcRect, &destRect);
			}
		}
	}
}