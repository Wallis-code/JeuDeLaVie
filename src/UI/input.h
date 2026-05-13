#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL.h>

typedef struct {
    int quit;
    // touches clavier
    int step;
    int auto_mode;
    // caméra
    int panning;   // maintien
    int pan_start_x, pan_start_y;
    int cam_start_x, cam_start_y;
    int cam_x, cam_y;
    int cell_size;
    // souris
    int mouse_left;
    int mouse_right;
    int mouse_x, mouse_y;
    // dev
    int win_model;
    
} Input;

void input_init(Input *in);
void input_update(Input *in, SDL_Event *e);
void input_return(Input *in);

#endif