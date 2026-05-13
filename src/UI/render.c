#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"


void render_grid(SDL_Renderer *r, Grid *g, int cam_x, int cam_y, int cell_size) {
    for (int i = 0; i < g->y; i++) {
        for (int j = 0; j < g->x; j++) {
            SDL_Rect cell = {
                cam_x + j * (cell_size + 1),
                cam_y + i * (cell_size + 1),
                cell_size,
                cell_size
            };
            // skip les cellules hors écran
            if (cell.x + cell_size < 0 || cell.x > 1250) continue;
            if (cell.y + cell_size < 0 || cell.y > 1250) continue;

            if (g->current[i][j] == ALIVE)
                SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
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

void render_editGrid(SDL_Renderer *r, Grid *g, int mx, int my,
                     int cam_x, int cam_y, int cell_size) {
    SDL_Point p = { mx, my };
    for (int i = 0; i < g->y; i++) {
        for (int j = 0; j < g->x; j++) {
            SDL_Rect cell = {
                cam_x + j * (cell_size + 1),
                cam_y + i * (cell_size + 1),
                cell_size,
                cell_size
            };
            // skip les cellules hors écran
            if (cell.x + cell_size < 0 || cell.x > 1250) continue;
            if (cell.y + cell_size < 0 || cell.y > 1250) continue;
            if (SDL_PointInRect(&p, &cell)) {
                Coord c; c.x = j; c.y = i;
                if ((g->current[i][j] == ALIVE))
                    grid_setDeadCurrent(g, c);
                else
                    grid_setAliveCurrent(g, c);
                return;
            }
        }
    }
}
