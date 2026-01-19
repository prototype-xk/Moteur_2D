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
		if (entry.id == id) {
			std::cout << "[WARNING] Texture '" << id << "' already loaded\n";
			return true;
		}
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
	for (auto& entry : fonts) {
		if (entry.id == id) {
			std::cout << "[WARNING] Font '" << id << "' already loaded\n";
			return true;
		}
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
	std::cout << "[SUCCESS] Loaded font '" << id << "' (" << (void*)font <<")\n";
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

bool ResourceManager::createTextTexture(std::string textureId, std::string fontId, const std::string& text, SDL_Color color) {
	for (auto& entry : textures) {
		if (entry.id == textureId) {
			std::cout << "[WARNING] Text texture '" << textureId << "' already exists\n";
			return true;
		}
	}

	TTF_Font* font = getFont(fontId);
	if (!font) {
		std::cerr << "[ERROR] Cannot create text texture '" << textureId
			<< "': font '" << fontId << "' not found\n";
		return false;
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, color);
	if (!textSurface) {
		std::cerr << "[ERROR] Failed to create text surface for '" << textureId
			<< "': " << SDL_GetError() << "\n";
		return false;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_DestroySurface(textSurface);

	if (!textTexture) {
		std::cerr << "[ERROR] Failed to create text texture for '" << textureId
			<< "': " << SDL_GetError() << "\n";
		return false;
	}

	TextureEntry entry;
	entry.id = textureId;
	entry.texture = textTexture;
	textures.push_back(entry);

	std::cout << "[SUCCESS] Created text texture '" << textureId << "' with text: \"" << text << "\"\n";
	return true;
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
		std::cout << "Freeing " << (void*)entry.fonts << std::endl;
		TTF_CloseFont(entry.fonts);
	}
	fonts.clear();
	std::cout << "[INFO] ResourceManager cleared\n";
}