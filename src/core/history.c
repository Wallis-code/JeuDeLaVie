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
    //le test supérieur à 100 ne sert à rien car normalement 
    //ça ne sera jamais le cas mais au cas ou je vais quand meme le mettre
    if(h->count > MAX){
        fprintf(stdout,"ERREUR - L'historique a trop d'état en mémoire");
        return true;
    }
    if(h->count == MAX){
        return true;
    }
    return false;
}


int history_getCount(History *h){return (h->count);}

void history_push(History *h, Grid *g){
    //on va reconstruire la grille, 
    //c'est vrm une photo d'une moment de la grille
    fprintf(stdout, "débug push\n");
    Grid *screen = grid_create(grid_getX(g) , grid_getY(g));
    if(!isFull(h)){
        for(int x = 0 ; x < grid_getY(screen) ; x++){
            for(int y = 0 ; y < grid_getX(screen) ; y++){
                screen->current[x][y] = g -> current[x][y];
            }
        }
        h->state[history_getCount(h)] = screen;
        h->count++;
    } else {
        grid_destroy(h->state[0]);  
        for (int i = 1; i < MAX; i++)
            h->state[i-1] = h->state[i];
        for (int x = 0; x < grid_getY(screen); x++)
            for (int y = 0; y < grid_getX(screen); y++)
                screen->current[x][y] = g->current[x][y];
        h->state[MAX - 1] = screen;
   }
}

void history_back(History *h, Grid *g) {
    //fprintf(stdout, "débug back\n");
    if (h->count <= 0){
        return;}
    //fprintf(stdout, "débug back APRES le test\n");
    h->count--;
    Grid *screen = h->state[h->count];
    for (int i = 0; i < g->y; i++)
        for (int j = 0; j < g->x; j++){
            g->current[i][j] = screen->current[i][j];
            g->next[i][j] = screen ->current[i][j];
        }
    }

void history_destroy(History *h) {
    if (h == NULL) return;
    for (int i = 0; i < h->count; i++)
        grid_destroy(h->state[i]);
    free(h->state);
    free(h);
}

