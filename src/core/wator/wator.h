#ifndef WATOR_H
#define WATOR_H

/*
je vais devoir refaire tout
ça va faire de la redondance de code,
mais tout caster en void* c'est atrocement long
*/

typedef enum { FISH , SHARK , DEAD} CellType;

typedef struct {
    CellType type;
    int hunger;
    int age;
}Cell;

//je simplifie un peu, c'est focément size*size en taille
//idéalement, c'est une sphère.
typedef struct {
    int size;
    Cell **current;
    Cell **next;
}WatorGrid;

//à voir si je garde ou pas

typedef struct{
    int x;
    int y;
}Coord;

WatorGrid *wator_create(int size);
void wator_destroy(WatorGrid *wg);


#endif