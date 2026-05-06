#include "core/grid.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    Grid *grid = grid_create(5,5);
    grid_print(grid);
    grid_destroy(grid);

    return EXIT_SUCCESS;
}