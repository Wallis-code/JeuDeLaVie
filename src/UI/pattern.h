#ifndef PATTERN_H
#define PATTERN_H

//pour les coordonnées
#include "core/grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum { SHIP1 , SHIP2 } Type;

typedef struct{
    Coord *form;
    //faut stocker le nombre de points dans le tableau
    int nb;
    Type type;
} Pattern;

typedef struct {
    SDL_Rect rect;
    const char *label;
    Pattern pattern;
} ButtonPattern;

void draw_buttonPattern(SDL_Renderer *r, TTF_Font *font, ButtonPattern *btn);
ButtonPattern *pattern_init(int *nb);
void pattern_destroy(ButtonPattern *buttons, int nb);

#endif