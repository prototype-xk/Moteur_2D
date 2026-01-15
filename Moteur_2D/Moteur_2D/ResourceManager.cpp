#include "ResourceManager.h"

ResourceManager::ResourceManager(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

ResourceManager::~ResourceManager() {
	clear();
}


bool ResourceManager::loadTexture(std::string id, std::string filePath) {
	for (auto& entry : textures) {
		if (entry.id == id)
			return true;
	}

	SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
	if (!tex) {
		return false;
	}

	TextureEntry entry;
	entry.id = id;
	entry.texture = tex;
	textures.push_back(entry);

	return true;
}

SDL_Texture* ResourceManager::getTexture(std::string id) {
	for (auto& entry : textures) {
		if (entry.id == id) {
			return entry.texture;
		}
	}
	return nullptr;
}

bool ResourceManager::loadFont(std::string id, std::string filePath, float size) {
	for (auto& entry : textures) {
		if (entry.id == id)
			return true;
	}

	TTF_Font* font = TTF_OpenFont(filePath.c_str(), size);
	if (!font) {
		return false;
	}

	FontEntry entry;
	entry.id = id;
	entry.fonts = font;
	fonts.push_back(entry);

	return true;
}

TTF_Font* ResourceManager::getFont(std::string id) {
	for (auto& entry : fonts) {
		if (entry.id == id)
			return entry.fonts;
	}
	return nullptr;
}

void ResourceManager::clear() {
	for (auto& entry : textures)
	{
		SDL_DestroyTexture(entry.texture);
	}
	textures.clear();

	for (auto& entry : fonts)
	{
		TTF_CloseFont(entry.fonts);
	}
	fonts.clear();
}