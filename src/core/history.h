#ifndef HISTORY_H
#define HISTORY_H

//max arbitraire
#define MAX 10000 

#include"grid.h"

//je vais stocker un tableau de grid.
//ça aurait pu être interessant de stocker que les coordonnées
//des points mais c'est trop aléatoire
//si y'a un point c'est méga rentable mais si y'a 2 cellules mortes pas du tout.
typedef struct{
    Grid **state;
    int count;
}History;

History *history_create();
void history_push(History *h, Grid *g);
void history_back(History *h, Grid *g);
void history_destroy(History *h);


#endif