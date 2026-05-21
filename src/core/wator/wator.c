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
    wg->next    = malloc(size * sizeof(Cell *));

    for (int i = 0; i < size; i++) {
        wg->current[i] = malloc(size * sizeof(Cell));
        wg->next[i]    = malloc(size * sizeof(Cell));
        for (int j = 0; j < size; j++) {
            wg->current[i][j] = cell_create(NO);
            wg->next[i][j]    = cell_create(NO);
        }
    }

    return wg;
}

void wator_destroy(WatorGrid *wg){
    for(int i = 0 ; i < wg->size ; i++){
            free(wg->current[i]);
            free(wg->next[i]);
    }
    free(wg->current);
    free(wg->next);
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
    wg ->next[c.y][c.x] = cell_create(NO);
}

void wator_setFish(WatorGrid *wg, Coord c){
    if(wg == NULL) return;
    if(!(check_coord(wg,c))) return;
    wg ->next[c.y][c.x] = cell_create(FISH);
}

void wator_setShark(WatorGrid *wg, Coord c){
    if(wg == NULL) return;
    if(!(check_coord(wg,c))) return;
    wg ->next[c.y][c.x] = cell_create(SHARK);
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

void wator_next(WatorGrid *wg){
    if(wg == NULL) return;
    Cell **tmp = wg ->current;
    wg->current = wg-> next;
    wg->next = tmp;
}

void wator_reset(WatorGrid *wg){
    for(int i = 0 ; i < wg->size ; i++){
        for(int j = 0 ; j < wg->size ; j++){
            wg->current[i][j] = cell_create(NO);
            wg->next[i][j] = cell_create(NO) ;
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

//coordonées autour
//je regarde les cases next car il est possible qu'a la
//génération suivante, un poisson ait des enfant ou
//se soit déplacé(tres probable en fait)
//ATTENTION : ça renvoie les 4 positions alentours, pas de tests ici
static Coord* allow(WatorGrid *wg, Coord c){
    if(!(check_coord(wg, c))) return NULL;
    
    //les 4 coordonées qui l'entourent, on va les passer modulo 3
    //parceque c'est une planête torroidale
    //les variables sont en français parceque c'est compliqué là
    Coord* tabCoo = malloc(sizeof(Coord) * 4);
    Coord haut,bas,gauche,droite;
    int size = wg->size;
    haut   = (Coord){coord_getX(c), (coord_getY(c) - 1 + size) % size};
    bas    = (Coord){coord_getX(c),  (coord_getY(c) + 1) % size};
    gauche = (Coord){(coord_getX(c) - 1 + size) % size, coord_getY(c)};
    droite = (Coord){(coord_getX(c) + 1) % size, coord_getY(c)};

    tabCoo[0] = haut;
    tabCoo[1] = bas;
    tabCoo[2] = gauche;
    tabCoo[3] = droite;

    return tabCoo;
}

static CellType get_next_type(WatorGrid *wg, Coord c) {
    if (!check_coord(wg, c)) return NO;
    return wg->next[c.y][c.x].type;
}

void wator_step(WatorGrid *wg) {
    if (wg == NULL) return;

    for (int i = 0; i < wg->size; i++)
        for (int j = 0; j < wg->size; j++)
            wg->next[i][j] = cell_create(NO);

    int *moved = calloc(wg->size * wg->size, sizeof(int));

    for (int i = 0; i < wg->size; i++) {
        for (int j = 0; j < wg->size; j++) {
            if (moved[i * wg->size + j]) continue;

            Coord tmp = {j, i};
            CellType type = wator_getCellType(wg, tmp);
            if (type == NO) continue;

            Coord *libre = allow(wg, tmp);
            if (libre == NULL) continue;

            if (type == FISH) {
                Cell fish = wator_getCell(wg, tmp);
                fish.age++;

                // cherche une case vide
                Coord step = {-1, -1};
                for (int k = 0; k < 4; k++)
                    if (get_next_type(wg, libre[k]) == NO) { step = libre[k]; break; }

                if (step.x == -1) {
                    wg->next[tmp.y][tmp.x] = fish;
                } else {
                    if (fish.age >= F_BREED) {
                        wg->next[tmp.y][tmp.x] = cell_create(FISH); // bébé
                        fish.age = 0;
                    }
                    wg->next[step.y][step.x] = fish;
                    moved[step.y * wg->size + step.x] = 1;
                }
            }

            else if (type == SHARK) {
                Cell shark = wator_getCell(wg, tmp);
                shark.age++;
                shark.hunger--;

                if (shark.hunger <= 0) { free(libre); continue; } // meurt

                // cherche poisson dans current
                Coord step = {-1, -1};
                for (int k = 0; k < 4; k++)
                    if (wator_getCellType(wg, libre[k]) == FISH) { step = libre[k]; break; }

                int reproduced = 0;
                if (step.x != -1) {
                    shark.hunger = STARVE; // mange
                } else {
                    // cherche case vide dans next
                    for (int k = 0; k < 4; k++)
                        if (get_next_type(wg, libre[k]) == NO) { step = libre[k]; break; }
                }

                if (shark.age >= S_BREED) {
                    wg->next[tmp.y][tmp.x] = cell_create(SHARK); // bébé
                    shark.age = 0;
                    reproduced = 1;
                }

                if (step.x != -1) {
                    wg->next[step.y][step.x] = shark;
                    moved[step.y * wg->size + step.x] = 1;
                } else {
                    if (!reproduced)
                        wg->next[tmp.y][tmp.x] = shark; // ne bouge pas
                }
            }

            free(libre);
        }
    }

    free(moved);
    wator_next(wg);
}

