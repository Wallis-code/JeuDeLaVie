#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/grid.h"
#include "core/history.h"
#include "UI/render.h"

/*-----------------BOUTON--------------------*/
typedef enum { PAS, AUTOMATIQUE, STOP, BACK, RESET } Action;

typedef struct {
    SDL_Rect rect;
    const char *label;
    Action action;
} Button;

void draw_button(SDL_Renderer *r, TTF_Font *font, Button *btn) {
    // fond
    SDL_SetRenderDrawColor(r, 100, 100, 100, 255);
    SDL_RenderFillRect(r, &btn->rect);
    // bordure
    SDL_SetRenderDrawColor(r, 150, 150, 220, 255);
    SDL_RenderDrawRect(r, &btn->rect);
    // texte
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, btn->label, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
    // centrer le texte dans le bouton
    int tw, th;
    TTF_SizeText(font, btn->label, &tw, &th);
    SDL_Rect dst = {
        btn->rect.x + (btn->rect.w - tw) / 2,
        btn->rect.y + (btn->rect.h - th) / 2,
        tw, th
    };
    SDL_RenderCopy(r, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *win = SDL_CreateWindow("Jeu de la vie",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1800, 1250, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    /*---------------DESSIN DES BOUTONS------------------*/
    // ils seront connectés aux algo après
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 20);
    if (font == NULL) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Button buttons[] = {
        { {1300, 50,  200, 100}, "UN PAS",      PAS        },
        { {1550, 50,  200, 100}, "AUTOMATIQUE", AUTOMATIQUE},
        { {1300, 200, 200, 100}, "STOP",        STOP},
        { {1550, 200, 200, 100}, "ARRIERE",     BACK},
        { {1300, 350, 200, 100}, "RESET",       RESET},


    };
    int nbButton = sizeof(buttons) / sizeof(buttons[0]);

    /*-----------------GRILLE---------------------------*/
    Grid *g = grid_create(50, 50);
    Coord coord[5] = {{2,2}, {3,3}, {3,4}, {2,4}, {1,4}};
    init(g, coord, 5);
    History *h = history_create();

    int auto_mode = 0;
    int mouse_pressed = 0;
    SDL_Event e;
    int running = 1;

    while (running) {
        /* --------------LA GESTION DES TOUCHES ------------------------*/
        while (SDL_PollEvent(&e)) {
            //edition
            render_editGrid(ren, g, &e);
            
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
            // S — avancer d'un step
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_S){
                history_push(h , g);
                grid_step(g);
                }
            // Q — lancer le mode auto
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_Q)
                auto_mode = 1;
            // L — stopper le mode auto
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_L)
                auto_mode = 0;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT){
                if(mouse_pressed){mouse_pressed = 1;}else{mouse_pressed = 0;}
                }
            /* --------------LA GESTION DES BOUTONS ------------------------*/
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point p = { e.button.x, e.button.y };
                for (int i = 0; i < nbButton; i++) {
                    if (SDL_PointInRect(&p, &buttons[i].rect)) {
                        if (buttons[i].action == PAS){
                            history_push(h , g);
                            grid_step(g);
                            }
                        if (buttons[i].action == AUTOMATIQUE) 
                            auto_mode = 1;
                        if (buttons[i].action == STOP) 
                            auto_mode = 0;
                        if (buttons[i].action == RESET) 
                            grid_reset(g);
                        if (buttons[i].action == BACK) 
                            history_back(h , g);
                    }
                }
            }
        }

        /*----------------PLUS DANS LA BOUCLE PRINCIPALE-----------*/
        if (auto_mode == 1) {
            history_push(h , g);
            grid_step(g);
            SDL_Delay(100);
        }
        if (mouse_pressed) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            render_editGridButton(ren, g,&e);
            }

        // rendu
        SDL_SetRenderDrawColor(ren, 20, 30, 60, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawLine(ren, 1250, 0, 1250, 1300);
        render_grid(ren, g);
        for (int i = 0; i < nbButton; i++)
            draw_button(ren, font, &buttons[i]);
        SDL_RenderPresent(ren);
    }

    // désallocation
    TTF_CloseFont(font);
    TTF_Quit();
    grid_destroy(g);
    history_destroy(h);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_SUCCESS;
}