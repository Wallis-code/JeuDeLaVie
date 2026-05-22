#include <assert.h>
#include <stdio.h>
#include "../src/core/history.h"
#include "../src/core/coord.h"

/*--------- je sais pas si j'ai le droit*/
//pour init + rapidement
static Grid* initTest(){
    Grid *g = grid_create(10,10);
    return g;
} 


void test_history_create() {
    History *h = history_create();
    assert(h != NULL);
    assert(h->count == 0);
    history_destroy(h);
}

void test_history_push_back() {
    Grid *g = initTest();
    History *h = history_create();

    // on met une cellule vivante
    Coord c = {3, 3};
    grid_setAlive(g, c);
    grid_next(g);

    history_push(h, g);
    assert(h->count == 1);

    // on modifie la grille
    grid_step(g);

    // on revient en arrière
    history_back(h, g);
    assert(g->current[3][3] == ALIVE);

    history_destroy(h);
    grid_destroy(g);
}

void test_history_limit() {
    Grid *g = initTest();
    History *h = history_create();

    // on pousse 150 états
    for (int i = 0; i < 150; i++)
        history_push(h, g);

    assert(h->count <= MAX);

    history_destroy(h);
    grid_destroy(g);
}

void test_history_back_vide() {
    Grid *g = initTest();
    History *h = history_create();

    
    history_back(h, g);
    assert(h->count == 0);

    history_destroy(h);
    grid_destroy(g);
}

int main() {
    test_history_create();
    test_history_push_back();
    test_history_limit();
    test_history_back_vide();
    printf("Tous les tests passent\n");
    return 0;
}