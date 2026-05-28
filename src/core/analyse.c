#define _POSIX_C_SOURCE 199309L
#include "analyse.h"
#include "grid.h"
#include "wator/wator.h"
#include "coord.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

//compte le nb de cellules vivantes
static int analyse_count_alive(Grid *g) {
    int count = 0;
    for (int i = 0; i < g->y; i++)
        for (int j = 0; j < g->x; j++)
            if (g->current[i][j] == ALIVE)
                count++;
    return count;
}

//max_gen pourrait etre remplacer par une fonction is_stable, qui break quand la population est stable
//mais pour l'instant ça marche comme ça
//je pense jamais le modifier honnêtement
void analyse_run(FILE *f, float density, int max_gen, int grid_size, int nb_runs) {
    // tableau pour accumuler la somme des populations
    //calloc initialise directement à 0, c'est + pratique
    int *sum_pop = calloc(max_gen, sizeof(int));

    //on va répéter plusieurs fois la simulation (par densité), pour avoir des données exploitables
    //et pas juste "un coup de chance"
    for (int run = 0; run < nb_runs; run++) {
        Grid *g = grid_create(grid_size, grid_size);
        grid_alea_density(g, density);

        for (int gen = 0; gen < max_gen; gen++) {
            sum_pop[gen] += analyse_count_alive(g);
            grid_step(g);
        }

        grid_destroy(g);
        printf("  densité %.0f%% - run %d/%d\n", density * 100, run + 1, nb_runs);
    }

    //une fois le tableau plein, on remplit le csv
    //on evite de faire trop d'ouverture
    for (int gen = 0; gen < max_gen; gen++)
        fprintf(f, "%.2f,%d,%d\n", density, gen, sum_pop[gen] / nb_runs);

    free(sum_pop);
}

void analyse_run_all(const char *filename, int max_gen, int grid_size, int nb_runs) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", filename);
        return;
    }

    // en-tête
    fprintf(f, "density,generation,population\n");

    // densités de 5% à 95% par pas de 5%
    float densities[] = {
        0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f,
        0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f,
        0.65f, 0.70f, 0.75f, 0.80f, 0.85f, 0.90f, 0.95f
    };
    int nb_densities = sizeof(densities) / sizeof(densities[0]);

    for (int i = 0; i < nb_densities; i++) {
        printf("Simulation densité %.0f%%\n", densities[i] * 100);
        analyse_run(f, densities[i], max_gen, grid_size, nb_runs);
    }

    fclose(f);
    printf("tout roule\n");
}




/*
######################################################################
##############################   ANALYSE WATOR   ######################
#######################################################################
*/

static void wator_alea(WatorGrid *wg) {
    for (int i = 0; i < wg->size; i++) {
        for (int j = 0; j < wg->size; j++) {
            int r = rand() % 100;
            Coord c = {j, i};
            if (r < 5)        wator_setShark(wg, c);  
            else if (r < 30)  wator_setFish(wg, c);   
        }
    }
}

static void analyse_wator_count(WatorGrid *wg,int *shark, int *fish){
    *shark = 0;
    *fish = 0;
    for(int i = 0 ; i< wg->size ; i++){
        for(int j = 0 ; j<wg->size ; j++){
            Coord c = { i , j};
            if(wator_getCellType(wg , c) == FISH)  (*fish)++;
            if(wator_getCellType(wg , c) == SHARK) (*shark)++;
        }
    }

}

void analyse_wator_run(const char *filename, int max_gen, int grid_size, int nb_runs) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) { 
        fprintf(stderr, "Erreur ouverture %s\n", filename); 
        return; 
    }
    fprintf(f, "generation,fish,shark\n");
    
    // tableau pour accumuler la somme des populations
    //calloc initialise directement à 0, c'est + pratique
    int *sum_fish = calloc(max_gen, sizeof(int));
    int *sum_shark = calloc(max_gen, sizeof(int));

    //on va répéter plusieurs fois la simulation, pour avoir des données exploitables
    //et pas juste "un coup de chance"
    for (int run = 0; run < nb_runs; run++) {
        WatorGrid *wg = wator_create(grid_size);
        wator_alea(wg);

        for (int gen = 0; gen < max_gen; gen++) {
            analyse_wator_count(wg,&sum_shark[gen] , &sum_fish[gen]);
            wator_step(wg);
        }

        wator_destroy(wg);
        printf("run %d/%d\n", run + 1, nb_runs);
    }

    //une fois le tableau plein, on remplit le csv
    //on evite de faire trop d'ouverture
    for (int gen = 0; gen < max_gen; gen++)
        fprintf(f, "%d,%d,%d\n", gen, sum_fish[gen] / nb_runs , sum_shark[gen] / nb_runs);

    free(sum_shark);
    free(sum_fish);
    fclose(f);
}

