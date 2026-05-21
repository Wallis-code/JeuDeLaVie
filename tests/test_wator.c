#include <assert.h>
#include <stdio.h>
#include "../src/core/wator/wator.h"
#include "../src/core/coord.h"


void test_wator_create() {
    WatorGrid *wg = wator_create(10);
    assert(wg != NULL);
    assert(wg->size == 10);
    // toutes les cases doivent être vides à la création
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            assert(wg->current[i][j].type == NO);
    wator_destroy(wg);
    printf("OK test_wator_create\n");
}

void test_wator_set_fish() {
    WatorGrid *wg = wator_create(10);
    wator_setFish(wg, ((Coord) {3,4}));
    assert(wator_getCellType(wg, ((Coord) {3,4})) == FISH);
    assert(wg->current[4][3].age == 0);
    assert(wg->current[4][3].hunger == -1);
    wator_destroy(wg);
    printf("OK test_wator_set_fish\n");
}

void test_wator_set_shark() {
    WatorGrid *wg = wator_create(10);
    wator_setShark(wg, ((Coord) {5,2}));
    assert(get(wg,((Coord) {5,2})) == SHARK);
    assert(wg->current[2][5].age == 0);
    assert(wg->current[2][5].hunger == STARVE);
    wator_destroy(wg);
    printf("OK test_wator_set_shark\n");
}

void test_wator_set_dead() {
    WatorGrid *wg = wator_create(10);
    wator_setFish(wg, ((Coord){3, 3}));
    assert(wator_getCellType(wg, ((Coord){3, 3})) == FISH);
    wator_setDead(wg, ((Coord){3, 3}));
    assert(wator_getCellType(wg, ((Coord){3, 3})) == NO);
    wator_destroy(wg);
    printf("OK test_wator_set_dead\n");
}

void test_wator_reset() {
    WatorGrid *wg = wator_create(10);
    wator_setFish(wg,((Coord){1, 1}));
    wator_setShark(wg,((Coord){5, 5}));
    wator_reset(wg);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            assert(wg->current[i][j].type == NO);
    wator_destroy(wg);
    printf("OK test_wator_reset\n");
}

void test_wator_coord_invalide() {
    WatorGrid *wg = wator_create(10);
    Coord hors = {99, 99};
    assert(wator_getCellType(wg, hors) == NO);
    wator_setFish(wg, hors);
    wator_destroy(wg);
    printf("OK test_wator_coord_invalide\n");
}

void test_wator_cell_create_fish() {
    Cell c = cell_create(FISH);
    assert(c.type == FISH);
    assert(c.age == 0);
    assert(c.hunger == -1);
    printf("OK test_wator_cell_create_fish\n");
}

void test_wator_cell_create_shark() {
    Cell c = cell_create(SHARK);
    assert(c.type == SHARK);
    assert(c.age == 0);
    assert(c.hunger == STARVE);
    printf("OK test_wator_cell_create_shark\n");
}

void test_wator_cell_create_no() {
    Cell c = cell_create(NO);
    assert(c.type == NO);
    assert(c.age == -1);
    assert(c.hunger == -1);
    printf("OK test_wator_cell_create_no\n");
}

int main() {
    test_wator_create();
    test_wator_set_fish();
    test_wator_set_shark();
    test_wator_set_dead();
    test_wator_reset();
    test_wator_coord_invalide();
    test_wator_cell_create_fish();
    test_wator_cell_create_shark();
    test_wator_cell_create_no();
    printf("Tous les tests passent\n");
    return 1;
}