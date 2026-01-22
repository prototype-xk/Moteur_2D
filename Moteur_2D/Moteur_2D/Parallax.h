#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>

class Parallax{
public:
	struct Layer {
		SDL_Texture* texture;
		float speedRatio=0; //0.1f = lent (fond), 0.8f = rapide(premier plan)
		float offsetX=0;
		float offsetY=0;
		float width=0;
		float height=0;
		float scale=0;
		float initialOffsetX=0; //décalage initial X
		float initialOffsetY=0; //décalage initial Y
	};

	Parallax(SDL_Renderer* renderer);
	~Parallax();

	bool addLayer(const std::string& imagePath, float speedRatio, float scale, float initialX = 0.0f, float initialY = 0.0f);
	void update(float deltaTime, float cameraX, float cameraY);
	void render(SDL_Renderer* renderer, float screenWidth, float screenHeight);

private:
	SDL_Renderer* m_renderer;
	std::vector<Layer> m_layers;
};

