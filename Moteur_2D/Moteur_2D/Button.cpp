#include "Button.h"

Button::Button(SDL_Texture* texture, float x, float y, float w, float h): m_texture(texture)
{
    baseRect = { x, y, w, h };
}

Button::~Button()
{
    if (m_textTexture) {
        SDL_DestroyTexture(m_textTexture);
        m_textTexture = nullptr;
    }
}

void Button::setText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    if (m_textTexture) {
        SDL_DestroyTexture(m_textTexture);
        m_textTexture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (!surface) {
        std::cerr << "[ERROR] TTF_RenderText_Blended (button text) failed: "<< SDL_GetError() << "\n";
        return;
    }

    m_textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!m_textTexture) {
        std::cerr << "[ERROR] SDL_CreateTextureFromSurface (button text) failed: "<< SDL_GetError() << "\n";
    }
}

void Button::handleEvent(const SDL_Event& e)
{
    if (e.type == SDL_EVENT_MOUSE_MOTION)
    {
        float mx = e.motion.x;
        float my = e.motion.y;

        bool inside =
            mx >= baseRect.x &&
            mx <= baseRect.x + baseRect.w &&
            my >= baseRect.y &&
            my <= baseRect.y + baseRect.h;
        m_isHovered = inside;
    }

    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
        float mx = e.button.x;
        float my = e.button.y;

        bool inside =
            mx >= baseRect.x &&
            mx <= baseRect.x + baseRect.w &&
            my >= baseRect.y &&
            my <= baseRect.y + baseRect.h;

        if (inside) {
            std::cout << "[BUTTON] Clicked on the button\n";
        }
    }
}


void Button::render(SDL_Renderer* renderer)
{
    SDL_FRect drawRect = baseRect;
    if (m_isHovered)
        drawRect.y += hoverOffsetY;
        
    if (m_texture) {
        SDL_RenderTexture(renderer, m_texture, nullptr, &drawRect);
    }
    else {
        SDL_SetRenderDrawColorRGBA(renderer, BTN_BACKGROUND_COLOR);
        SDL_RenderFillRect(renderer, &drawRect);
    }

    if (m_textTexture) {
        float tw = 0.f, th = 0.f;
        if (SDL_GetTextureSize(m_textTexture, &tw, &th)) {
            SDL_FRect textDst;
            textDst.w = tw;
            textDst.h = th;
            textDst.x = drawRect.x + (drawRect.w - tw) / 2.0f;
            textDst.y = drawRect.y + (drawRect.h - th) / 2.0f;

            SDL_RenderTexture(renderer, m_textTexture, nullptr, &textDst);
        }
    }
}
