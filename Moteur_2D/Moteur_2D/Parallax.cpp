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

		// ? CALCUL DYNAMIQUE : combien de copies horizontalement ?
		int copiesX = (int)ceilf((screenWidth + drawW) / drawW);
		int copiesY = (int)ceilf((screenHeight + drawH) / drawH);

		// ? BOUCLE INFINIE HORIZONTALE/VERTicale
		for (int y = -1; y <= copiesY; y++) {
			for (int x = -1; x <= copiesX; x++) {
				float drawX = layer.offsetX + (x * drawW);
				float drawY = layer.offsetY + (y * drawH);

				// Optimisation : ne dessine que ce qui est visible
				if (drawX + drawW > 0 && drawX < screenWidth &&
					drawY + drawH > 0 && drawY < screenHeight) {

					SDL_FRect destRect = { drawX, drawY, drawW, drawH };
					SDL_RenderTexture(renderer, layer.texture, &srcRect, &destRect);
				}
			}
		}
	}
}