#ifndef WATOR_H
#define WATOR_H
#include "core/coord.h"

/*

https://en.wikipedia.org/wiki/Wa-Tor
https://beltoforion.de/en/wator/

je prendrais les règles originelles,pas celles
de beltoforion
*/


/*
je vais devoir refaire tout
ça va faire de la redondance de code,
mais tout caster en void* c'est atrocement long
*/


/*
* Au bout de combien de temps un poisson
* se reproduit
*/
#define F_BREED 3
//idem pour le requin
#define S_BREED 10

/*
* La faim de base d'un requin
* décrémente s'il mange pas
* si 0 alors meurt
*/
#define STARVE 10
// 


typedef enum { FISH , SHARK , NO} CellType;

typedef struct {
    CellType type;
    /*
      uniquement pour les requins, dernier repas
      incrémente à chaque generation/pas
      sans manger, reviens à 0 quand il a mangé.   
      */
    int hunger;   
    int age;      //age depuis la naissance
}Cell;

//je simplifie un peu, c'est focément size*size en taille
//idéalement, c'est une sphère.
typedef struct {
    int size;
    Cell **current;
    Cell **next;
}WatorGrid;


Cell cell_create(CellType ct);
WatorGrid *wator_create(int size);
void wator_destroy(WatorGrid *wg);
void wator_setDead(WatorGrid *wg, Coord c);
void wator_setFish(WatorGrid *wg, Coord c);
void wator_setShark(WatorGrid *wg, Coord c);
Cell wator_getCell(WatorGrid *wg, Coord c);
CellType wator_getCellType(WatorGrid *wg , Coord c);
void wator_next(WatorGrid *wg);
void wator_reset(WatorGrid *wg);
void wator_step(WatorGrid *wg);

#endif