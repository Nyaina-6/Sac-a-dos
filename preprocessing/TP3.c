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
    read_TP2_instance(fin, &data);
    fclose(fin);
   
    int* selected = knapsack_preprocessing(data, data.n, &data.b);

    printf("Selected items after preprocessing:\n");
    for (int i = 0; i < data.n; i++) {
        if (selected[i])
            printf("Item %d (weight: %d, value: %d)\n", i, data.a[i], data.c[i]);
    }

    free(selected);
    
    return  EXIT_SUCCESS;
}