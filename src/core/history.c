#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

History *history_create(){
    History *h = malloc(sizeof(History));
    if(h == NULL){
        printf("ERREUR - l'historique ne s'est pas créé\n");
        return NULL;
    } 
    h->state = malloc(sizeof(Grid *) * MAX);
    h->count = 0;
    return h;
}

static bool isFull(History *h){
    //le test supérieur à 1000 ne sert à rien car normalement 
    //ça ne sera jamais le cas mais au cas ou je vais quand meme le mettre
    if(h->count > MAX){
        //printf("ERREUR - L'historique a trop d'état en mémoire");
        return false;
    }
    if(h->count == MAX){
        return false;
    }
    return true;
}

void history_push(History *h, Grid *g){
    //on va reconstruire la grille, 
    //c'est vrm une photo d'une moment de la grille
    Grid *screen = grid_create(grid_getX(g) , grid_getY(g));
    for(int x = 0 ; x < grid_getY(screen) ; x++){
        for(int y = 0 ; y < grid_getX(screen) ; y++){
            screen->current[x][y] = g -> current[x][y];
        }
    }
    int nb = h->count;
    h->state[nb];
}
