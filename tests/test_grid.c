#include <assert.h>
#include <stdio.h>
#include "../src/core/grid.h"

void test_grid_create(){
    Grid *g = grid_create(10,10);
    assert(g != NULL);
    assert(g ->x == 10);
    assert(g ->y == 10);
    grid_destroy(g);
    printf("OK test_grid_create\n");
}

int main(){
    test_grid_create();
    return 0;

}