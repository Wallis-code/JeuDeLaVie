#include "pattern.h"
#include <stdlib.h>
#include "core/coord.h"


void draw_buttonPattern(SDL_Renderer *r, TTF_Font *font, ButtonPattern *btn) {
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

ButtonPattern *pattern_init(int *nb) {
    *nb = 11;
    ButtonPattern *buttons = malloc(sizeof(ButtonPattern) * (*nb));

    // Glider — vaisseau diagonal, 4 générations par cycle
    Coord *glider = malloc(sizeof(Coord) * 5);
    glider[0]=(Coord){1,0}; glider[1]=(Coord){2,1};
    glider[2]=(Coord){0,2}; glider[3]=(Coord){1,2}; glider[4]=(Coord){2,2};

    // LWSS — vaisseau léger horizontal, période 4
    Coord *lwss = malloc(sizeof(Coord) * 9);
    lwss[0]=(Coord){1,0}; lwss[1]=(Coord){4,0};
    lwss[2]=(Coord){0,1};
    lwss[3]=(Coord){0,2}; lwss[4]=(Coord){4,2};
    lwss[5]=(Coord){0,3}; lwss[6]=(Coord){1,3};
    lwss[7]=(Coord){2,3}; lwss[8]=(Coord){3,3};

    // Pulsar — oscillateur période 3, le plus commun
    Coord *pulsar = malloc(sizeof(Coord) * 48);
    int pi = 0;
    int cols1[] = {2,3,4,8,9,10};
    int rows1[] = {0,5,7,12};
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 6; c++)
            pulsar[pi++] = (Coord){cols1[c], rows1[r]};
    int cols2[] = {0,5,7,12};
    int rows2[] = {2,3,4,8,9,10};
    for (int r = 0; r < 6; r++)
        for (int c = 0; c < 4; c++)
            pulsar[pi++] = (Coord){cols2[c], rows2[r]};

    // Pentadecathlon — oscillateur période 15
    Coord *penta = malloc(sizeof(Coord) * 12);
    penta[0]=(Coord){1,0};  penta[1]=(Coord){1,1};
    penta[2]=(Coord){0,2};  penta[3]=(Coord){2,2};
    penta[4]=(Coord){1,3};  penta[5]=(Coord){1,4};
    penta[6]=(Coord){1,5};  penta[7]=(Coord){1,6};
    penta[8]=(Coord){0,7};  penta[9]=(Coord){2,7};
    penta[10]=(Coord){1,8}; penta[11]=(Coord){1,9};

    // R-pentomino — méthuselah, 5 cellules, se stabilise après 1103 générations
    Coord *rpento = malloc(sizeof(Coord) * 5);
    rpento[0]=(Coord){1,0}; rpento[1]=(Coord){2,0};
    rpento[2]=(Coord){0,1}; rpento[3]=(Coord){1,1};
    rpento[4]=(Coord){1,2};

    // Diehard — 7 cellules, disparait complètement après 130 générations
    Coord *diehard = malloc(sizeof(Coord) * 7);
    diehard[0]=(Coord){6,0};
    diehard[1]=(Coord){0,1}; diehard[2]=(Coord){1,1};
    diehard[3]=(Coord){1,2}; diehard[4]=(Coord){5,2};
    diehard[5]=(Coord){6,2}; diehard[6]=(Coord){7,2};

    // Acorn — 7 cellules, se stabilise après 5206 générations en produisant 13 gliders
    Coord *acorn = malloc(sizeof(Coord) * 7);
    acorn[0]=(Coord){1,0};
    acorn[1]=(Coord){3,1};
    acorn[2]=(Coord){0,2}; acorn[3]=(Coord){1,2};
    acorn[4]=(Coord){4,2}; acorn[5]=(Coord){5,2}; acorn[6]=(Coord){6,2};

    // Gosper Glider Gun — premier canon connu, produit un glider toutes les 30 générations
    Coord *gun = malloc(sizeof(Coord) * 36);
    int gi = 0;
    gun[gi++]=(Coord){24,0};
    gun[gi++]=(Coord){22,1}; gun[gi++]=(Coord){24,1};
    gun[gi++]=(Coord){12,2}; gun[gi++]=(Coord){13,2};
    gun[gi++]=(Coord){20,2}; gun[gi++]=(Coord){21,2};
    gun[gi++]=(Coord){34,2}; gun[gi++]=(Coord){35,2};
    gun[gi++]=(Coord){11,3}; gun[gi++]=(Coord){15,3};
    gun[gi++]=(Coord){20,3}; gun[gi++]=(Coord){21,3};
    gun[gi++]=(Coord){34,3}; gun[gi++]=(Coord){35,3};
    gun[gi++]=(Coord){0,4};  gun[gi++]=(Coord){1,4};
    gun[gi++]=(Coord){10,4}; gun[gi++]=(Coord){16,4};
    gun[gi++]=(Coord){20,4}; gun[gi++]=(Coord){21,4};
    gun[gi++]=(Coord){0,5};  gun[gi++]=(Coord){1,5};
    gun[gi++]=(Coord){10,5}; gun[gi++]=(Coord){14,5};
    gun[gi++]=(Coord){16,5}; gun[gi++]=(Coord){17,5};
    gun[gi++]=(Coord){22,5}; gun[gi++]=(Coord){24,5};
    gun[gi++]=(Coord){10,6}; gun[gi++]=(Coord){16,6};
    gun[gi++]=(Coord){24,6};
    gun[gi++]=(Coord){11,7}; gun[gi++]=(Coord){15,7};
    gun[gi++]=(Coord){12,8}; gun[gi++]=(Coord){13,8};

    // HWSS — vaisseau lourd horizontal, période 4
    Coord *hwss = malloc(sizeof(Coord) * 13);
    hwss[0]=(Coord){2,0};  hwss[1]=(Coord){3,0};
    hwss[2]=(Coord){0,1};  hwss[3]=(Coord){1,1};
    hwss[4]=(Coord){4,1};  hwss[5]=(Coord){5,1};
    hwss[6]=(Coord){0,2};  hwss[7]=(Coord){1,2};
    hwss[8]=(Coord){2,2};  hwss[9]=(Coord){3,2};
    hwss[10]=(Coord){4,2}; hwss[11]=(Coord){5,2};
    hwss[12]=(Coord){1,3};

   Coord *pi_hept = malloc(sizeof(Coord) * 7);
    pi_hept[0]=(Coord){0,0}; pi_hept[1]=(Coord){1,0}; pi_hept[2]=(Coord){2,0};
    pi_hept[3]=(Coord){0,1}; pi_hept[4]=(Coord){2,1};
    pi_hept[5]=(Coord){0,2}; pi_hept[6]=(Coord){2,2};

    // Infinite growth — 10 cellules, croissance infinie
    Coord *inf = malloc(sizeof(Coord) * 10);
    inf[0]=(Coord){0,0};
    inf[1]=(Coord){1,0};
    inf[2]=(Coord){2,0};
    inf[3]=(Coord){4,0};
    inf[4]=(Coord){0,1};
    inf[5]=(Coord){3,2};
    inf[6]=(Coord){4,2};
    inf[7]=(Coord){1,3};
    inf[8]=(Coord){2,3};
    inf[9]=(Coord){4,3};

    // placement — 2 colonnes de 5
    int bw = 210, bh = 55, gap = 8, x0 = 10, y0 = 10;
    buttons[0] = (ButtonPattern){ {x0,        y0+0*(bh+gap), bw, bh}, "Glider",         {glider,  5,  SHIP1} };
    buttons[1] = (ButtonPattern){ {x0+bw+gap, y0+0*(bh+gap), bw, bh}, "LWSS",           {lwss,    9,  SHIP2} };
    buttons[2] = (ButtonPattern){ {x0,        y0+1*(bh+gap), bw, bh}, "Pulsar (p3)",    {pulsar,  48, SHIP1} };
    buttons[3] = (ButtonPattern){ {x0+bw+gap, y0+1*(bh+gap), bw, bh}, "Pentadecathlon", {penta,   12, SHIP2} };
    buttons[4] = (ButtonPattern){ {x0,        y0+2*(bh+gap), bw, bh}, "R-pentomino",    {rpento,  5,  SHIP1} };
    buttons[5] = (ButtonPattern){ {x0+bw+gap, y0+2*(bh+gap), bw, bh}, "Diehard",        {diehard, 7,  SHIP2} };
    buttons[6] = (ButtonPattern){ {x0,        y0+3*(bh+gap), bw, bh}, "Acorn",          {acorn,   7,  SHIP1} };
    buttons[7] = (ButtonPattern){ {x0+bw+gap, y0+3*(bh+gap), bw, bh}, "Gosper Gun",     {gun,     36, SHIP2} };
    buttons[8] = (ButtonPattern){ {x0,        y0+4*(bh+gap), bw, bh}, "HWSS",           {hwss,    13, SHIP1} };
    buttons[9] = (ButtonPattern){ {x0+bw+gap, y0+4*(bh+gap), bw, bh}, "Croissance inf", {inf,     10, SHIP2} };
    buttons[10] = (ButtonPattern){ {x0, y0+5*(bh+gap), bw, bh}, "Pi heptomino", {pi_hept, 7, SHIP1} };

    return buttons;
}

void pattern_destroy(ButtonPattern *buttons, int nb) {
    for (int i = 0; i < nb; i++)
        free(buttons[i].pattern.form);
    free(buttons);
}
