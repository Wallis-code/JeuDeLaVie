#define _POSIX_C_SOURCE 199309L

#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

int coord_getX(Coord c){return c.x;}
int coord_getY(Coord c){return c.y;}

Grid *grid_create(int x, int y)
{
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

int grid_getX(Grid *g){return g->x;}
int grid_getY(Grid *g){return g->y;}

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
    if (c.x >= grid_getX(grid) || c.x < 0
       || c.y >= grid_getY(grid) || c.y < 0){
        //printf("ERREUR -- Les coordonnées ne sont pas dans la grille");
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
    grid ->next[c.y][c.x] = DEAD;
}

void grid_setAlive(Grid *grid,Coord c){
    if(grid == NULL) return;
    if(!(check_coord(grid,c))) return;
    grid ->next[c.y][c.x] = ALIVE;
}

int grid_getState(Grid *g, Coord point){
    if(!(check_coord(g,point)))
        return -1;
    return g->current[coord_getX(point)][coord_getY(point)]; 
}

void grid_next(Grid *grid){
    if(grid == NULL) return;
    int **tmp = grid ->current;
    grid ->current = grid->next;
    grid ->next = tmp;
}

void init(Grid* grid,Coord coords[], int nbCoords){
    if(!(check_coord_array(grid,coords,nbCoords)))
        return;
    for(int i = 0 ; i< nbCoords ; i++){
        grid_setAlive(grid,coords[i]);
    }
    grid_next(grid);
}

/*---------------
---------------------TOUT CE QUI CONCERNE L'ETAPE SUIVANTE
---------------*/

//attention on ne test pas si le point en question est DEAD or ALIVE
//on renvoie juste son nb de voisin en vie
//on va regarder les 8 cases autours, ex : on teste 10 10, il faut regarder
/*
    9 9  , 9 10  , 9 11
    10 9 ,  X    , 10 11
    11 9 , 11 10 , 11 11

*/ 
static int grid_pointNeighborCoords(Grid *g,Coord point){
    //test pour savoir si le point de base est bien dans la grille
    if(!check_coord(g , point)) return -1;
    
    int count = 0;
    Coord tmp;

    for (int i = coord_getX(point) - 1; i <= coord_getX(point) + 1; i++) {
        for (int j = coord_getY(point) - 1; j <= coord_getY(point) + 1; j++) {
            //cas de base
            if (i == coord_getX(point) && j == coord_getY(point)) continue;
            tmp.x = i;
            tmp.y = j;
            if (check_coord(g, tmp) && g->current[j][i] == 1)
                count++;
        }
    }
    return count;
}

void grid_step(Grid *g){
    if (g==NULL)
     return;

    for (int i = 0; i < grid_getY(g); i++) {
        for (int j = 0; j < grid_getX(g); j++) {
                Coord tmp;
                tmp.x = j;
                tmp.y = i;
                int ngb = grid_pointNeighborCoords(g , tmp);
                if( ngb == 3){
                    g->next[i][j] = ALIVE; 
                }
                if(ngb == 2){
                    g->next[i][j] = g->current[i][j]; 
                }
                if((ngb < 2) || (ngb >3)){
                    g->next[i][j] = DEAD; 
                }
        }
    }
    grid_next(g);
}

static void mySleep(float sec) {
    struct timespec ts;
    ts.tv_sec = (int)sec;
    ts.tv_nsec = (sec - (int)sec) * 1000000000L;
    nanosleep(&ts, NULL);
}


void grid_automatic(Grid *g,int boucle,float clock){
    for(int i = 0 ; i< boucle ; i++){
    system("clear");   
    grid_step(g);
    grid_print(g);
    mySleep(clock);
    }

}