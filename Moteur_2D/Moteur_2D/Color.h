#pragma once

#include <SDL3/SDL.h>

#define BTN_BACKGROUND_COLOR  0xffc4c4c4
#define PLAYER_JUMP_COLOR 0xff00ff00 //Vert
#define PLAYER_LEFT_COLOR 0xffffff00 //Jaune
#define PLAYER_RIGHT_COLOR 0xffff00c8 //Rose
#define PLAYER_IDLE_COLOR 0xffff0000 //Rouge

#define RGBA_ALPHA(color)   ((Uint8)(((color)>>24)&0xff))
#define RGBA_RED(color)     ((Uint8)(((color)>>16)&0xff))
#define RGBA_GREEN(color)   ((Uint8)(((color)>> 8)&0xff))
#define RGBA_BLUE(color)    ((Uint8)(((color)>> 0)&0xff))

inline void SDL_SetRenderDrawColorRGBA(SDL_Renderer* renderer, Uint32 color)
{
    SDL_SetRenderDrawColor(renderer, RGBA_RED(color), RGBA_GREEN(color), RGBA_BLUE(color), RGBA_ALPHA(color));
}