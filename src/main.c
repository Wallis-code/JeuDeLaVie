#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/grid.h"
#include "core/history.h"
#include "UI/render.h"
#include "UI/input.h"
#include "UI/ui.h"
#include "UI/pattern.h"

int main() {

    /*-------------TEST ERREURS SDL-------------------------*/
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
        { {1300, 200, 200, 100}, "STOP",        STOP       },
        { {1550, 200, 200, 100}, "ARRIERE",     BACK       },
        { {1300, 350, 200, 100}, "RESET",       RESET      },
        { {1150, 25,  75,   50}, "PERDU",       PERDU      },
    };
    int nbButton = sizeof(buttons) / sizeof(buttons[0]);

    int nbPattern = 0;
    ButtonPattern *buttonsPattern = pattern_init(&nbPattern);

    /*-----------------GRILLE---------------------------*/
    Grid *g = grid_create(500, 500);
    Coord coord[5] = {{2,2}, {3,3}, {3,4}, {2,4}, {1,4}};
    init(g, coord, 5);
    History *h = history_create();

    /*-----------------INPUT---------------------------*/
    Input in;
    input_init(&in);

    /*-----------------FENETRE PATTERN---------------------------*/
    SDL_Window   *win2 = NULL;
    SDL_Renderer *ren2 = NULL;
    Uint32 main_win_id = SDL_GetWindowID(win);
    ButtonPattern *selected_pattern = NULL;

    /*-----------------SDL---------------------------*/
    SDL_Event e;
    int running = 1;
    Uint32 last_step = 0;
    int step_delay = 100;

    while (running) {
        /* --------------LA GESTION DES TOUCHES ------------------------*/
        while (SDL_PollEvent(&e)) {
            input_update(&in, &e);

            // fermeture fenetre pattern
            if (e.type == SDL_WINDOWEVENT
                && e.window.event == SDL_WINDOWEVENT_CLOSE
                && win2 != NULL
                && e.window.windowID == SDL_GetWindowID(win2)) {
                SDL_DestroyRenderer(ren2);
                SDL_DestroyWindow(win2);
                win2 = NULL;
                ren2 = NULL;
                in.win_model = 0;
            }

            // fermeture fenetre principale
            if (e.type == SDL_WINDOWEVENT
                && e.window.event == SDL_WINDOWEVENT_CLOSE
                && e.window.windowID == main_win_id)
                running = 0;

            // edition au clic gauche — pose pattern ou édition cellule
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT
                && e.button.windowID == main_win_id
                && e.button.x < 1250 && !in.panning) {

                if (selected_pattern != NULL) {
                    // convertir coordonnées écran -> grille
                    int col = (in.mouse_x - in.cam_x) / (in.cell_size + 1);
                    int row = (in.mouse_y - in.cam_y) / (in.cell_size + 1);
                    // poser le pattern
                    for (int i = 0; i < selected_pattern->pattern.nb; i++) {
                        Coord c;
                        c.x = col + selected_pattern->pattern.form[i].x;
                        c.y = row + selected_pattern->pattern.form[i].y;
                        grid_setAliveCurrent(g, c);
                    }
                    selected_pattern = NULL;  // désélectionner après pose
                } else {
                    // édition normale
                    render_editGrid(ren, g, in.mouse_x, in.mouse_y,
                                    in.cam_x, in.cam_y, in.cell_size);
                }
            }

            /* --------------LA GESTION DES BOUTONS ------------------------*/
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.windowID == main_win_id) {
                SDL_Point p = { e.button.x, e.button.y };
                for (int i = 0; i < nbButton; i++) {
                    if (SDL_PointInRect(&p, &buttons[i].rect)) {
                        if (buttons[i].action == PAS) {
                            history_push(h, g);
                            grid_step(g);
                        }
                        if (buttons[i].action == AUTOMATIQUE) in.auto_mode = 1;
                        if (buttons[i].action == STOP)        in.auto_mode = 0;
                        if (buttons[i].action == RESET)       grid_reset(g);
                        if (buttons[i].action == BACK)        history_back(h, g);
                        if (buttons[i].action == PERDU)       input_return(&in);
                    }
                }
            }

            /*CLIQUE SUR UN PATTERN*/
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT
                && win2 != NULL && e.button.windowID == SDL_GetWindowID(win2)) {
                SDL_Point p = { e.button.x, e.button.y };
                for (int i = 0; i < nbPattern; i++) {
                    if (SDL_PointInRect(&p, &buttonsPattern[i].rect))
                        selected_pattern = &buttonsPattern[i];
                }
            }
        }

        /*----------------PLUS DANS LA BOUCLE PRINCIPALE-----------*/
        if (in.step) {
            history_push(h, g);
            grid_step(g);
        }

        // anti-lag mode auto — SDL_GetTicks évite le SDL_Delay bloquant
        if (in.auto_mode == 1) {
            Uint32 now = SDL_GetTicks();
            if (now - last_step >= (Uint32)step_delay) {
                history_push(h, g);
                grid_step(g);
                last_step = now;
            }
        }

        if (in.quit) running = 0;

        // edition clic gauche maintenu
        // marche pas car les cellules passent dans les deux états trop vite
        if (in.mouse_left && in.mouse_x < 1250 && win2 == NULL)
            render_editGrid(ren, g, in.mouse_x, in.mouse_y,
                            in.cam_x, in.cam_y, in.cell_size);

        // fenetre pattern
        if (in.win_model && win2 == NULL) {
            in.win_model = 0;
            win2 = SDL_CreateWindow("Pattern", 2250, 185, 450, 380, SDL_WINDOW_SHOWN);
            ren2 = SDL_CreateRenderer(win2, -1, SDL_RENDERER_ACCELERATED);
        }

        // rendu fenetre principale
        SDL_SetRenderDrawColor(ren, 20, 30, 60, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawLine(ren, 1290, 0, 1290, 1300);
        render_grid(ren, g, in.cam_x, in.cam_y, in.cell_size);
        for (int i = 0; i < nbButton; i++)
            draw_button(ren, font, &buttons[i]);
        SDL_RenderPresent(ren);

        // rendu fenetre pattern
        if (win2 != NULL) {
            SDL_SetRenderDrawColor(ren2, 20, 30, 60, 255);
            SDL_RenderClear(ren2);
            for (int j = 0; j < nbPattern; j++)
                draw_buttonPattern(ren2, font, &buttonsPattern[j]);
            SDL_RenderPresent(ren2);
        }
    }

    // désallocation
    if (win2 != NULL) {
        SDL_DestroyRenderer(ren2);
        SDL_DestroyWindow(win2);
    }
    pattern_destroy(buttonsPattern, nbPattern);
    TTF_CloseFont(font);
    TTF_Quit();
    grid_destroy(g);
    history_destroy(h);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_SUCCESS;
}