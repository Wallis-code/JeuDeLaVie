#include <assert.h>
#include <stdio.h>
#include "../src/core/grid.h"

void test_grid_create() {
    Grid *g = grid_create(10, 10);
    assert(g != NULL);
    assert(g->x == 10);
    assert(g->y == 10);
    grid_destroy(g);
    printf("OK test_grid_create\n");
}

void test_grid_create_zero() {
    Grid *g = grid_create(10, 10);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            assert(g->current[i][j] == 0);
    grid_destroy(g);
    printf("OK test_grid_create_zero\n");
}

void test_grid_setAlive() {
    Grid *g = grid_create(10, 10);
    Coord c; c.x = 3; c.y = 3;
    grid_setAlive(g, c);
    grid_next(g);
    assert(g->current[3][3] == ALIVE);
    grid_destroy(g);
    printf("OK test_grid_setAlive\n");
}

void test_grid_setDead() {
    Grid *g = grid_create(10, 10);
    Coord c; c.x = 3; c.y = 3;
    grid_setAlive(g, c);
    grid_next(g);
    grid_setDead(g, c);
    grid_next(g);
    assert(g->current[3][3] == DEAD);
    grid_destroy(g);
    printf("OK test_grid_setDead\n");
}

void test_grid_check_coord_valid() {
    Grid *g = grid_create(10, 10);
    Coord c; c.x = 5; c.y = 5;
    assert(grid_getState(g, c) == 0);
    grid_destroy(g);
    printf("OK test_grid_check_coord_valid\n");
}

void test_grid_check_coord_invalid() {
    Grid *g = grid_create(10, 10);
    Coord c; c.x = 15; c.y = 15;
    assert(grid_getState(g, c) == -1);
    grid_destroy(g);
    printf("OK test_grid_check_coord_invalid\n");
}

void test_blinker() {
    // blinker vertical gen 0 -> horizontal gen 1
    Grid *g = grid_create(10, 10);
    Coord c1; c1.x = 4; c1.y = 3;
    Coord c2; c2.x = 4; c2.y = 4;
    Coord c3; c3.x = 4; c3.y = 5;
    grid_setAlive(g, c1);
    grid_setAlive(g, c2);
    grid_setAlive(g, c3);
    grid_next(g);
    grid_step(g);
    assert(g->current[4][3] == ALIVE);
    assert(g->current[4][4] == ALIVE);
    assert(g->current[4][5] == ALIVE);
    assert(g->current[3][4] == DEAD);
    assert(g->current[5][4] == DEAD);
    grid_destroy(g);
    printf("OK test_blinker\n");
}

void test_underpopulation() {
    // cellule avec 1 voisin meurt
    Grid *g = grid_create(10, 10);
    Coord c1; c1.x = 4; c1.y = 4;
    Coord c2; c2.x = 4; c2.y = 5;
    grid_setAlive(g, c1);
    grid_setAlive(g, c2);
    grid_next(g);
    grid_step(g);
    assert(g->current[4][4] == DEAD);
    assert(g->current[4][5] == DEAD);
    grid_destroy(g);
    printf("OK test_underpopulation\n");
}

void test_overpopulation() {
    // cellule avec 4 voisins meurt
    Grid *g = grid_create(10, 10);
    Coord c;
    c.x = 4; c.y = 4; grid_setAlive(g, c);
    c.x = 3; c.y = 4; grid_setAlive(g, c);
    c.x = 5; c.y = 4; grid_setAlive(g, c);
    c.x = 4; c.y = 3; grid_setAlive(g, c);
    c.x = 4; c.y = 5; grid_setAlive(g, c);
    grid_next(g);
    grid_step(g);
    assert(g->current[4][4] == DEAD);
    grid_destroy(g);
    printf("OK test_overpopulation\n");
}

int main() {
    test_grid_create();
    test_grid_create_zero();
    test_grid_setAlive();
    test_grid_setDead();
    test_grid_check_coord_valid();
    test_grid_check_coord_invalid();
    test_blinker();
    test_underpopulation();
    test_overpopulation();
    printf("Tous les tests passent\n");
    return 0;
}