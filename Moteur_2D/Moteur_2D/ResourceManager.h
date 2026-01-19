#pragma once

#include <vector>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

struct TextureEntry {
	std::string id;
	SDL_Texture* texture;
};

struct FontEntry {
	std::string id;
	TTF_Font* fonts;
};

class ResourceManager
{
public:
	ResourceManager(SDL_Renderer* renderer);
	~ResourceManager();

	bool loadTexture(std::string id, std::string filePath);
	SDL_Texture* getTexture(std::string id);

	bool loadFont(std::string id, std::string filePath, float size);
	TTF_Font* getFont(std::string id);

	void clear();
private:
	SDL_Renderer* renderer;
	std::vector<TextureEntry> textures;
	std::vector<FontEntry> fonts;

	bool hasLoggedTextureError;
	bool hasLoggedFontError;
};

