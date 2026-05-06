#ifndef GRID_H
#define GRIF_H

/*

        x
    -----------------          ----------------
    |              |           |              |     
  y |      ...     |     -->   |       .      |
    |              |           |       .      |
    |              |           |       .      |
   ------------------          ----------------
        current                       next


Pour que ça soit clair j'ai mis des points ici,
mais ça représente un tableau d'entier :
                                        - 0 : cellule morte
                                        - 1 : cellule vivante

*/
typedef struct{
    int x;
    int y;
    int **current;
    int **next;
}Grid;

typedef struct{
    int x;
    int y;
}Coord;

Grid *grid_create(int x, int y);
void grid_print(Grid *grid);
void grid_destroy(Grid *grid);
void grid_setDead(Grid *grid,Coord c);
void grid_setAlive(Grid *grid,Coord c);
void init(Grid* grid,Coord coords[], int nbCoords);



#endif