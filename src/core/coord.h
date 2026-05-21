#ifndef COORD_H
#define COORD_H

typedef struct{
    int x;
    int y;
}Coord;



int coord_getX(Coord c);
int coord_getY(Coord c);

#endif