#ifndef GRID_H
#define GRID_H

#define DEAD 0
#define ALIVE 1

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

int coord_getX(Coord c);
int coord_getY(Coord c);

Grid *grid_create(int x, int y);
int grid_getX(Grid *g);
int grid_getY(Grid *g);
void grid_print(Grid *grid);
void grid_destroy(Grid *grid);
void grid_setDead(Grid *grid,Coord c);
void grid_setAlive(Grid *grid,Coord c);
int grid_getState(Grid *g,Coord point);
void grid_next(Grid *grid);
void init(Grid* grid,Coord coords[], int nbCoords);
void grid_step(Grid *g);
void grid_automatic(Grid *g,int boucle,float clock);
void grid_clear(Grid *g);
void grid_setDeadCurrent(Grid *grid,Coord c);
void grid_setAliveCurrent(Grid *grid,Coord c);
void grid_reset(Grid *grid);





#endif