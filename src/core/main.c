#include "core/grid.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    Grid *grid = grid_create(20,20);
    Coord coord[5] = {{2,2} , {3,3} , {3,4} , {2,4} , {1,4}};
    init(grid , coord , 5);

    grid_automatic(grid , 100 , 0.005);
    

    grid_destroy(grid);

    return EXIT_SUCCESS;
}