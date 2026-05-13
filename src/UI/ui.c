#include "ui.h"


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
