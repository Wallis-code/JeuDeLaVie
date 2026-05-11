#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/grid.h"

#define CELL_DIMENSION 20
#define CELL_SEPARATOR 5
#define GRID_DIMENSION 700

void render_grid(SDL_Renderer *r, Grid *g);
void render_step(Grid *g);
void render_automatic(Grid *g,Uint32 ms);


#endif