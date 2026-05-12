#include "render.h"
#include <stdio.h>
#include <stdlib.h>

void render_grid(SDL_Renderer *r, Grid *g) {
    for (int i = 0; i < g->y; i++) {
        for (int j = 0; j < g->x; j++) {
            SDL_Rect cell = {
                j * (CELL_DIMENSION + CELL_SEPARATOR),
                i * (CELL_DIMENSION + CELL_SEPARATOR),
                CELL_DIMENSION,
                CELL_DIMENSION
            };
            if (g->current[i][j] == ALIVE){
                SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            }else{
                SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
            }
            SDL_RenderFillRect(r, &cell);
        }
    }
}

void render_step(Grid *g){
    grid_step(g);
}

void render_automatic( Grid *g, Uint32 ms){
    render_step(g);
    SDL_Delay(ms);
}

void render_editGrid(SDL_Renderer *r, Grid *g, SDL_Event *e) {
    if ((e->type != SDL_MOUSEBUTTONDOWN)) return;

    SDL_Point p = { e->button.x, e->button.y };

    for (int i = 0; i < g->y; i++) {
        for (int j = 0; j < g->x; j++) {
            SDL_Rect cell = {
                j * (CELL_DIMENSION + CELL_SEPARATOR),
                i * (CELL_DIMENSION + CELL_SEPARATOR),
                CELL_DIMENSION,
                CELL_DIMENSION
            };
            if (SDL_PointInRect(&p, &cell)) {
                Coord c; c.x = j; c.y = i;
                if (g->current[i][j] == ALIVE)
                    grid_setDeadCurrent(g,c);
                else
                    grid_setAliveCurrent(g,c);
                return;
            }
        }
    }
}

void render_editGridButton(SDL_Renderer *r, Grid *g, SDL_Event *e){
    SDL_Point p = { e->button.x, e->button.y };

    for (int i = 0; i < g->y; i++) {
        for (int j = 0; j < g->x; j++) {
            SDL_Rect cell = {
                j * (CELL_DIMENSION + CELL_SEPARATOR),
                i * (CELL_DIMENSION + CELL_SEPARATOR),
                CELL_DIMENSION,
                CELL_DIMENSION
            };
            if (SDL_PointInRect(&p, &cell)) {
                Coord c; c.x = j; c.y = i;
                if (g->current[i][j] == ALIVE)
                    grid_setDeadCurrent(g,c);
                else
                    grid_setAliveCurrent(g,c);
                return;
            }
        }
    }
}
