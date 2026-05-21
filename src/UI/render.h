#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/grid.h"
#include "core/wator/wator.h"

void render_grid(SDL_Renderer *r, Grid *g, int cam_x, int cam_y, int cell_size);
void render_step(Grid *g);
void render_automatic(Grid *g,Uint32 ms);
void render_editGrid(SDL_Renderer *r, Grid *g, int mx, int my,int cam_x, int cam_y, int cell_size);
void render_wator(SDL_Renderer *r, WatorGrid *wg, int cam_x, int cam_y, int cell_size);
#endif