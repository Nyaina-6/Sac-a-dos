/* TP1.c */
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "TP1Functions.h"

#include <bits/getopt_core.h>

int main(int argc, char** argv) {
    int rval = 0;
    // File instance name
    //-F option
    char instance_file[1024];
    snprintf(instance_file,
             1024,
             "%s",
             "instance3.csv");

    char c;
    while ((c = getopt(argc, argv, "F:h")) != EOF) {
        switch (c) {
            case 'F':
                snprintf(instance_file,
                         1024,
                         "%s",
                         optarg);
                break;

            case 'h':
                fprintf(stderr, "Usage: ./TP1 [options]\nOptions:\n\n");
                fprintf(stderr, "******** INSTANCE DATA ********\n");
                fprintf(stderr, "\t-F Instance file name to load............................(default %s).\n", instance_file);
                break;
            default:
                exit(0);
        }
    }

    dataSet data;

    FILE* fin = fopen(instance_file, "r");
    read_TP1_instance(fin, &data);
    fclose(fin);

	int* greedy_solution = KP_greedy(&data);

    printf("Objets pris dans la solution de l'algorithme glouton:\n");
    int greedy_total_value = 0;
    int greedy_total_weight = 0;
    for (int i = 0; i < data.n; ++i) {
        if (greedy_solution[i] == 1) {
            printf("Objet %d : poids = %d, valeur = %d\n", i + 1, data.a[i], data.c[i]);
            greedy_total_value += data.c[i];
            greedy_total_weight += data.a[i];
        }
    }
    printf("Poids total des objets pris par l'algorithme glouton : %d\n", greedy_total_weight);
    printf("Valeur totale des objets pris par l'algorithme glouton : %d\n\n", greedy_total_value);

    double* LP_solution = KP_LP(&data);

    // Afficher la solution de relaxation linéaire pour vérification
    printf("Objets pris dans la solution de l'algorithme LP:\n");
    double total_value = 0;
    double total_weight = 0;
    for (int i = 0; i < data.n; ++i) {
        if (LP_solution[i] > 0) {
            printf("Objet %d : poids = %.2f, valeur = %.2f,  fraction  = %.2f\n", i , data.a[i]*LP_solution[i], data.c[i]*LP_solution[i] , LP_solution[i]);
            total_value += data.c[i]*LP_solution[i];
            total_weight += data.a[i]*LP_solution[i];
        }
    }
    printf("Poids total des objets pris par l'algorithme relaxation linéaire : %.2f\n", total_weight);
    printf("Valeur totale des objets pris par l'algorithme relaxation linéaire : %.2f\n\n", total_value);

    free(LP_solution);
    free(greedy_solution);
    return rval;
}
