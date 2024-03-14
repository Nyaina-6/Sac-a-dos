#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "prog.h"


int main(int argc, char** argv) {
    // File instance name
    //-F option
    char instance_file[1024];
    snprintf(instance_file,
             1024,
             "%s",
             "instances.csv");

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
    read_TP2_instance(fin, &data);
    fclose(fin);

    /**FILE* file = fopen("instances.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    generate_instance(file);
        fclose(file);*/
    printf("Resolution dynamique normale \n");
    knapsack(data);
    printf("\n\n");
    printf("Resolution dynamique en triant les variables \n");
    knapsack_tri(data);

   
    free(data.c);
    free(data.a);

    return  EXIT_SUCCESS;
}