#include "wator.h"
#include "../coord.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


Cell cell_create(CellType ct){
    Cell c;
    
    if(ct == SHARK){
        c.age = 0;
        c.hunger = STARVE;
        c.type = ct;  
    }else if(ct == FISH){
        c.age = 0;
        //pas d'hunger chezles poissons
        c.hunger = -1;
        c.type = ct;
    }else{
        //pas d'age sur les cases vides
        c.age = -1;
        //pas d'hunger chezles poissons
        c.hunger = -1;
        c.type = ct;
    }
    return c;
}

WatorGrid *wator_create(int size)
{
    WatorGrid *wg = malloc(sizeof(WatorGrid));
    if(wg == NULL) return NULL;

    wg->size = size;
    wg->current = malloc(size * sizeof(Cell *));

    for (int i = 0; i < size; i++) {
        wg->current[i] = malloc(size * sizeof(Cell));
        for (int j = 0; j < size; j++) {
            wg->current[i][j] = cell_create(NO);
        }
    }

    return wg;
}

void wator_destroy(WatorGrid *wg){
    for(int i = 0 ; i < wg->size ; i++){
            free(wg->current[i]);
    }
    free(wg->current);
    free(wg);
}

static bool check_coord(WatorGrid* wg, Coord c){
    if (c.x >= wg->size || c.x < 0
       || c.y >= wg->size || c.y < 0){
        //printf("ERREUR -- Les coordonnées ne sont pas dans la grille");
        return false;
    }else{ return true;}
}

void wator_setDead(WatorGrid *wg, Coord c){
    if(wg == NULL) return;
    if(!(check_coord(wg,c))) return;
    wg ->current[c.y][c.x] = cell_create(NO);
}

void wator_setFish(WatorGrid *wg, Coord c){
    if(wg == NULL) return;
    if(!(check_coord(wg,c))) return;
    wg ->current[c.y][c.x] = cell_create(FISH);
}

void wator_setShark(WatorGrid *wg, Coord c){
    if(wg == NULL) return;
    if(!(check_coord(wg,c))) return;
    wg ->current[c.y][c.x] = cell_create(SHARK);
}

Cell wator_getCell(WatorGrid *wg, Coord c){
    if(!(check_coord(wg,c))) return cell_create(NO);
    return wg->current[coord_getY(c)][coord_getX(c)];
}

CellType wator_getCellType(WatorGrid *wg , Coord c){
    if(!(check_coord(wg,c))) return NO;
    Cell cell = wg->current[coord_getY(c)][coord_getX(c)];
    return cell.type;
}

void wator_reset(WatorGrid *wg){
    for(int i = 0 ; i < wg->size ; i++){
        for(int j = 0 ; j < wg->size ; j++){
            wg->current[i][j] = cell_create(NO);
        }
    }
}

/*---------------
---------------------TOUT CE QUI CONCERNE LA GEN SUIVANTE
---------------*/

//on va regarder les 8 cases autours, ex : on teste 10 10, il faut regarder
//MAIS C'EST UNE PLANETE EN FORME DE TOR, FAUT PAS OUBLIER
/*
           10 9  
    9 10 ,  X    , 11 10
           10 11 

*/


/*
Algo adapté et "copié" d'un dépôt github en c++ , ligne 639
https://github.com/beltoforion/Wator-Screensaver/blob/master/WatorWnd.cpp

*/

void wator_step(WatorGrid *wg) {
    if (wg == NULL) return;

    int size = wg->size;
    int *moved = calloc(size * size, sizeof(int));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (moved[i * size + j]) continue;

            Coord dirs[4] = {{0,-1},{0,1},{-1,0},{1,0}};
            int start = rand() % 4;

            /*----- CAS SHARK-----*/
            if (wg->current[i][j].type == SHARK) {
                Cell shark = wg->current[i][j];
                shark.age++;
                shark.hunger--;

                if (shark.hunger <= 0) {
                    wg->current[i][j] = cell_create(NO);
                    continue;
                }

                Coord new_co = {-1, -1};
                int ate = 0;

                for (int k = 0; k < 4; k++) {
                    int d = (start + k) % 4;
                    Coord voisin = {
                        (j + dirs[d].x + size) % size,
                        (i + dirs[d].y + size) % size
                    };
                    if (wg->current[voisin.y][voisin.x].type == FISH) {
                        new_co = voisin;
                        ate = 1;
                        break;
                    }
                }

                if (new_co.x == -1) {
                    for (int k = 0; k < 4; k++) {
                        int d = (start + k) % 4;
                        Coord voisin = {
                            (j + dirs[d].x + size) % size,
                            (i + dirs[d].y + size) % size
                        };
                        if (wg->current[voisin.y][voisin.x].type == NO) {
                            new_co = voisin;
                            break;
                        }
                    }
                }

                if (ate) shark.hunger = STARVE;

                if (shark.age >= S_BREED) {
                    wg->current[i][j] = cell_create(SHARK);
                    shark.age = 0;
                    //on marque la case du bébé requin
                    moved[i * size + j] = 1;
                } else {
                    wg->current[i][j] = cell_create(NO);
                }

                if (new_co.x != -1) {
                    wg->current[new_co.y][new_co.x] = shark;
                    moved[new_co.y * size + new_co.x] = 1;
                } else {
                    wg->current[i][j] = shark;
                }
            }

            /*----- CAS FISH ENSUITE -----*/
            else if (wg->current[i][j].type == FISH) {
                Coord new_co = {-1, -1};

                for (int k = 0; k < 4; k++) {
                    int d = (start + k) % 4;
                    Coord voisin = {
                        (j + dirs[d].x + size) % size,
                        (i + dirs[d].y + size) % size
                    };
                    if (wg->current[voisin.y][voisin.x].type == NO) {
                        new_co = voisin;
                        break;
                    }
                }

                if (new_co.x == -1) continue;

                Cell fish = wg->current[i][j];
                fish.age++;

                if (fish.age >= F_BREED) {
                    wg->current[i][j] = cell_create(FISH);
                    fish.age = 0;
                } else {
                    wg->current[i][j] = cell_create(NO);
                }

                wg->current[new_co.y][new_co.x] = fish;
                moved[new_co.y * size + new_co.x] = 1;
            }
        }
    }

    free(moved);
}