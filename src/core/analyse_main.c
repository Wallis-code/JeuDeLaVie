#include "analyse.h"
#include <stdlib.h>

int main(void) {
    // 19 densités
    // grille 100*100
    //je laisse 500 generations pour se stabilisé,
    //c'est arbitraire, dans mes tests précédents, ça dépassait pas 450
    analyse_run_all("simulation.csv", 500, 100, 100);
    return EXIT_SUCCESS;
}
