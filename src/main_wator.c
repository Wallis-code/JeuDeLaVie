#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/wator/wator.h"
#include "core/coord.h"
#include "UI/render.h"
#include "UI/input.h"
#include "UI/ui.h"
#include "core/coord.h"


int main() {

    /*-------------TEST ERREURS SDL-------------------------*/
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *win = SDL_CreateWindow("Wa-Tor",
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
        { {1300, 200, 200, 100}, "STOP",        STOP       },
        { {1550, 200, 200, 100}, "RESET",       RESET      },
        { {1300, 350, 200, 100}, "ALEATOIRE",   RANDOM     },
        { {1150, 25,  75,   50}, "PERDU",       PERDU      },
    };
    int nbButton = sizeof(buttons) / sizeof(buttons[0]);

    /*-----------------GRILLE WATOR---------------------------*/
    WatorGrid *wg = wator_create(50);
    // initialisation avec 2 poissons, qui vont se reproduire en balle
    //et 2 requins, on va prier pour que les requins tombent sur les poissons
    Coord fish1 = {10, 10};
    Coord fish2 = {12, 10};
    Coord shark1 = {20, 20};
    Coord shark2 = {11, 10};

    wator_setFish(wg, fish1);
    wator_setFish(wg, fish2);
    wator_setShark(wg, shark1);
    wator_setShark(wg, shark2);
    wator_setShark(wg, (Coord) {22, 20});
    wator_next(wg);

    /*-----------------INPUT---------------------------*/
    Input in;
    input_init(&in);

    /*-----------------SDL---------------------------*/
    Uint32 main_win_id = SDL_GetWindowID(win);
    SDL_Event e;
    int running   = 1;
    Uint32 last_step = 0;
    int step_delay   = 100;

    while (running) {
        /* --------------LA GESTION DES TOUCHES ------------------------*/
        while (SDL_PollEvent(&e)) {
            input_update(&in, &e);

            // fermeture fenetre principale
            if (e.type == SDL_WINDOWEVENT
                && e.window.event == SDL_WINDOWEVENT_CLOSE
                && e.window.windowID == main_win_id)
                running = 0;

            /* --------------LA GESTION DES BOUTONS ------------------------*/
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.windowID == main_win_id) {
                SDL_Point p = { e.button.x, e.button.y };
                for (int i = 0; i < nbButton; i++) {
                    if (SDL_PointInRect(&p, &buttons[i].rect)) {
                        if (buttons[i].action == PAS)       wator_step(wg);
                        if (buttons[i].action == AUTOMATIQUE) in.auto_mode = 1;
                        if (buttons[i].action == STOP)        in.auto_mode = 0;
                        if (buttons[i].action == RESET)       wator_reset(wg);
                        if (buttons[i].action == PERDU)       input_return(&in);
                        if (buttons[i].action == RANDOM) {
                            wator_reset(wg);
                            for (int ii = 0; ii < 500; ii++) {
                                for (int jj = 0; jj < 500; jj++) {
                                    int r = rand() % 10;
                                    Coord c = {jj, ii};
                                    if (r == 0)      wator_setShark(wg, c);
                                    else if (r <= 3) wator_setFish(wg, c);
                                }
                            }
                            wator_next(wg);
                        }
                    }
                }
            }
        }

        /*----------------PLUS DANS LA BOUCLE PRINCIPALE-----------*/
        if (in.step) wator_step(wg);

        // anti-lag mode auto
        if (in.auto_mode == 1) {
            Uint32 now = SDL_GetTicks();
            if (now - last_step >= (Uint32)step_delay) {
                wator_step(wg);
                last_step = now;
            }
        }

        if (in.quit) running = 0;

        // rendu fenetre principale
        SDL_SetRenderDrawColor(ren, 20, 30, 60, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawLine(ren, 1290, 0, 1290, 1300);
        render_wator(ren, wg, in.cam_x, in.cam_y, in.cell_size);
        for (int i = 0; i < nbButton; i++)
            draw_button(ren, font, &buttons[i]);
        SDL_RenderPresent(ren);
    }

    // désallocation
    TTF_CloseFont(font);
    TTF_Quit();
    wator_destroy(wg);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_SUCCESS;
}