#include "input.h"

void input_init(Input *in) {
    in->quit        = 0;
    in->step        = 0;
    in->auto_mode   = 0;
    in->panning     = 0;
    in->pan_start_x = 0;
    in->pan_start_y = 0;
    in->cam_start_x = 0;
    in->cam_start_y = 0;
    in->cam_x       = 0;
    in->cam_y       = 0;
    in->cell_size   = 4;
    in->mouse_left  = 0;
    in->mouse_right = 0;
    in->mouse_x     = 0;
    in->mouse_y     = 0;
    // dev
    in->win_model   = 0;
}

void input_update(Input *in, SDL_Event *e) {
    in->step = 0;

    if (e->type == SDL_QUIT) in->quit = 1;
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE) in->quit = 1;

    // S - avancer d'un step
    if (e->type == SDL_KEYDOWN && e->key.keysym.scancode == SDL_SCANCODE_S)
        in->step = 1;
    // Q - lancer le mode auto
    if (e->type == SDL_KEYDOWN && e->key.keysym.scancode == SDL_SCANCODE_Q)
        in->auto_mode = 1;
    // L - stopper le mode auto
    if (e->type == SDL_KEYDOWN && e->key.keysym.scancode == SDL_SCANCODE_L)
        in->auto_mode = 0;

    // zoom molette
    if (e->type == SDL_MOUSEWHEEL) {
        if (e->wheel.y > 0 && in->cell_size < 40) in->cell_size++;
        if (e->wheel.y < 0 && in->cell_size > 1)  in->cell_size--;
    }

    // pan clic droit maintenu
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_RIGHT) {
        in->panning     = 1;
        in->mouse_right = 1;
        in->pan_start_x = e->button.x;
        in->pan_start_y = e->button.y;
        in->cam_start_x = in->cam_x;
        in->cam_start_y = in->cam_y;
    }
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_RIGHT) {
        in->panning     = 0;
        in->mouse_right = 0;
    }
    if (e->type == SDL_MOUSEMOTION && in->panning) {
        in->cam_x = in->cam_start_x + (e->motion.x - in->pan_start_x);
        in->cam_y = in->cam_start_y + (e->motion.y - in->pan_start_y);
    }

    // clic gauche
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        in->mouse_left = 1;
        in->mouse_x    = e->button.x;
        in->mouse_y    = e->button.y;
    }
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
        in->mouse_left = 0;

    // position souris en temps réel
    if (e->type == SDL_MOUSEMOTION) {
        in->mouse_x = e->motion.x;
        in->mouse_y = e->motion.y;
    }

    // dev
    if (e->type == SDL_KEYDOWN && e->key.keysym.scancode == SDL_SCANCODE_W){
        in->win_model = 1;
        }
}

void input_return(Input *in) {
    in->cam_x = 0;
    in->cam_y = 0;
}