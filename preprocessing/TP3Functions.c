#include <stdio.h>
#include <stdlib.h>
#include "prog.h"

int read_TP2_instance(FILE* fin, dataSet* dsptr) {
    int rval = 0;

    // Capacite b
    int b;
    // Nombre d'objets
    int n;
    rval = fscanf(fin, "%d,%d\n", &n, &b);
    dsptr->b = b;
    dsptr->n = n;
    dsptr->c = (int*)malloc(sizeof(int) * n);
    dsptr->a = (int*)malloc(sizeof(int) * n);

    int i;
    for (i = 0; i < n; i++)
        rval = fscanf(fin, "%d,%d\n", &(dsptr->c[i]), &(dsptr->a[i]));

    fprintf(stderr, "\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
            b, n);
    for (i = 0; i < n; i++)
        fprintf(stderr, "%d,%d\n", dsptr->c[i], dsptr->a[i]);
    fprintf(stderr, "\n");

    return rval;
}

int compare(const void* a, const void* b) {
    Objet* objA = (Objet*)a;
    Objet* objB = (Objet*)b;
    if (objB->utility - objA->utility > 0) return 1;
    if (objB->utility - objA->utility < 0) return -1;
    return 0;
}

int* KP_greedy(dataSet* dsptr) {
    
    Objet* utilities = malloc(sizeof(Objet) * dsptr->n);

    for (int i = 0; i < dsptr->n; ++i) {
        Objet ob;
        ob.index = i;
        ob.utility = dsptr->c[i] / (double)dsptr->a[i];
        utilities[i] = ob;
    }

    qsort(utilities, dsptr->n, sizeof(Objet), compare);

    int* solution = calloc(dsptr->n, sizeof(int));

    int remaining_capacity = dsptr->b;


    for (int j = 0; j < dsptr->n; ++j) {
        if (remaining_capacity == 0) {
            break;
        }
        if (remaining_capacity >= dsptr->a[utilities[j].index]) {
            solution[utilities[j].index] = 1;
            remaining_capacity -= dsptr->a[utilities[j].index];
        }
    }

    free(utilities); 
    return solution;
}

double* KP_LP(dataSet* dsptr) {
    Objet* utilities = malloc(sizeof(Objet) * dsptr->n);
    
    for (int i = 0; i < dsptr->n; ++i) {
        Objet ob;
        ob.index = i;
        ob.utility = dsptr->c[i] / (double)dsptr->a[i];
        utilities[i] = ob;
    }

    qsort(utilities, dsptr->n, sizeof(Objet), compare); 
    double* solution = calloc(dsptr->n, sizeof(int)); 
    double remaining_capacity = dsptr->b; 
    double quantity = 0;

    
    for (int j = 0; j < dsptr->n; ++j) {
        if (remaining_capacity == 0) {
            break; 
        }
            double min_quantity = (double)remaining_capacity / dsptr->a[utilities[j].index];
            printf  ("%f, %d\n", min_quantity, dsptr->a[utilities[j].index]);
            if (min_quantity >= 1) {
                quantity = 1;
            } 
            else {
                quantity = min_quantity ; }
                solution[utilities[j].index] = quantity;
                remaining_capacity -= quantity * dsptr->a[utilities[j].index];
    }

    free(utilities); 
    return solution; 
}

