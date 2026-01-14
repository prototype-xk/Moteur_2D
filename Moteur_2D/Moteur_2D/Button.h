#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "Color.h"

class Button
{
public:
    Button(SDL_Texture* texture, float x, float y, float w, float h);
    ~Button();

    void handleEvent(const SDL_Event& e);
    void render(SDL_Renderer* renderer);

    bool isHovered() const { return m_isHovered; }

    void setText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);

private:
    SDL_FRect baseRect;
    float hoverOffsetY = 3.0f;
    bool m_isHovered = false;
    SDL_Texture* m_texture = nullptr;
    SDL_Texture* m_textTexture = nullptr;
};