#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


Grid *grid_create(int x, int y){
    Grid *grid = malloc(sizeof(Grid));
    if (grid==NULL)
     return NULL;
    
    grid -> x = x;
    grid -> y = y;

    grid->current = malloc(y * sizeof(int *));
    grid->next    = malloc(y * sizeof(int *));

    for (int i = 0; i < y; i++) {
        grid->current[i] = malloc(x * sizeof(int));
        grid->next[i]    = malloc(x * sizeof(int));
        for (int j = 0; j < x; j++) {
            grid->current[i][j] = 0;
            grid->next[i][j]    = 0;
        }
    }

    return grid;
}

void grid_print(Grid *grid){
    for(int i = 0 ; i < grid->x ; i++){
        for(int j = 0 ; j < grid->y ; j++){
            if(grid->current[i][j] == 0){
                printf("-");
            }else{
                printf("0");
            }
        }
        printf("\n"); 
    }
}

void grid_destroy(Grid *grid){
    for(int i = 0 ; i < grid->x ; i++){
            free(grid->current[i]);
            free(grid->next[i]);
    }
    free(grid->current);
    free(grid->next);
    free(grid);
}

static bool check_coord(Grid* grid, Coord c){
    if((c.x >grid->x) | (c.x < 0)
     | (c.y >grid->y) | (c.y <0)
    ){
        printf("ERREUR -- Les coordonnées ne sont pas dans la grille");
        return false;
    }else{

    return true;
    }
}

static bool check_coord_array(Grid* grid, Coord c[],int nbCoords){
    for (int i = 0; i < nbCoords-1 ; i++){
        if(!(check_coord(grid,c[i])))
            return false;
    }
    return true;
}

void grid_setDead(Grid *grid,Coord c){
    if(grid == NULL) return;
    if(!(check_coord(grid,c))) return;
    grid ->next[c.x][c.y] = 0;
}

void grid_setAlive(Grid *grid,Coord c){
    if(grid == NULL) return;
    if(!(check_coord(grid,c))) return;
    grid ->next[c.x][c.y] = 1;
}


void init(Grid* grid,Coord coords[], int nbCoords){
    if(!(check_coord_array(grid,coords,nbCoords)))
        return;
    for(int i = 0 ; i< nbCoords ; i++){
        grid_setAlive(grid,coords[i]);
    }
}
