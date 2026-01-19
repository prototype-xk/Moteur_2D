#include "ResourceManager.h"
#include <iostream>
ResourceManager::ResourceManager(SDL_Renderer* renderer)
	: renderer(renderer)
	, hasLoggedTextureError(false)
	, hasLoggedFontError(false) {
	std::cout << "[INFO] ResourceManager created\n";
}

ResourceManager::~ResourceManager() {
	clear();
}


bool ResourceManager::loadTexture(std::string id, std::string filePath) {
	for (auto& entry : textures) {
		if (entry.id == id)
			std::cout << "[WARNING] Texture '" << id << "' already loaded\n";
			return true;
	}
	std::cout << "[INFO] Loading texture '" << id << "' from '" << filePath << "'...\n";

	SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
	if (!tex) {
		std::cerr << "[ERROR] Failed to load texture '" << id << "' from '"
			<< filePath << "': " << SDL_GetError() << "\n";
		return false;
	}

	TextureEntry entry;
	entry.id = id;
	entry.texture = tex;
	textures.push_back(entry);
	std::cout << "[SUCCESS] Loaded texture '" << id << "'\n";
	return true;
}

SDL_Texture* ResourceManager::getTexture(std::string id) {
	for (auto& entry : textures) {
		if (entry.id == id) {
			return entry.texture;
		}
	}

	if (!hasLoggedTextureError) {
		std::cerr << "[ERROR] Texture '" << id << "' not found in ResourceManager\n";
		hasLoggedTextureError = true;
	}
	return nullptr;
}

bool ResourceManager::loadFont(std::string id, std::string filePath, float size) {
	for (auto& entry : textures) {
		if (entry.id == id)
			std::cout << "[WARNING] Font '" << id << "' already loaded\n";
			return true;
	}

	std::cout << "[INFO] Loading font '" << id << "' from '" << filePath << "'...\n";
	TTF_Font* font = TTF_OpenFont(filePath.c_str(), size);
	if (!font) {
		std::cerr << "[ERROR] Failed to load font '" << id << "' from '" << filePath << "': " << SDL_GetError() << "\n";
		return false;
	}

	FontEntry entry;
	entry.id = id;
	entry.fonts = font;
	fonts.push_back(entry);
	std::cout << "[SUCCESS] Loaded font '" << id << "'\n";
	return true;
}

TTF_Font* ResourceManager::getFont(std::string id) {
	for (auto& entry : fonts) {
		if (entry.id == id)
			return entry.fonts;
	}
	if (!hasLoggedFontError) {
		std::cerr << "[ERROR] Font '" << id << "' not found in ResourceManager\n";
		hasLoggedFontError = true;
	}
	return nullptr;
}

void ResourceManager::clear() {
	std::cout << "[INFO] Clearing ResourceManager...\n";
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
	std::cout << "[INFO] ResourceManager cleared\n";
}