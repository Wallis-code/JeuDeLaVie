#ifndef ANALYSE_H
#define ANALYSE_H

#include "wator/wator.h"
#include "grid.h"
#include <stdio.h>

// une ligne du CSV
//deprecated
typedef struct {
    float density;
    int generation;
    int population;
} DataPoint;

// lance une simulation complète et écrit dans le fichier CSV
void analyse_run(FILE *f, float density, int max_gen, int grid_size,int nb_runs);

// lance toutes les simulations pour toutes les densités
void analyse_run_all(const char *filename, int max_gen, int grid_size, int nb_runs);

//wator
void analyse_wator_run(const char *filename, int max_gen, int grid_size, int nb_runs);


#endif