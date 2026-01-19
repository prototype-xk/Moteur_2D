#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>

class Parallax{
public:
	struct Layer {
		SDL_Texture* texture;
		float speedRatio; //0.1f = lent (fond), 0.8f = rapide(premier plan)
		float offsetX;
		float offsetY;
		float width;
		float height;
	};

	Parallax(SDL_Renderer* renderer);
	~Parallax();

	bool addLayer(const std::string& imagePath, float speedRatio);
	void update(float deltaTime, float playerX, float playerY);
	void render(SDL_Renderer* renderer, float screenWidth, float screenHeight);

private:
	SDL_Renderer* m_renderer;
	std::vector<Layer> m_layers;
};

