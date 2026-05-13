#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum { PAS, AUTOMATIQUE, STOP, BACK, RESET, PERDU } Action;

typedef struct {
    SDL_Rect rect;
    const char *label;
    Action action;
} Button;

void draw_button(SDL_Renderer *r, TTF_Font *font, Button *btn);

#endif