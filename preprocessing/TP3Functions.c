#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
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

void writeSelectedObjectsToCSV(dataSet data, int* selection, int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (selection[i] == -1) {
            fprintf(file, "%d,%d\n", data.c[i], data.a[i]);
        }
    }

    fclose(file);
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

double* KP_LP(dataSet data) {
    Objet* utilities = malloc(sizeof(Objet) * data.n);
    
    for (int i = 0; i < data.n; ++i) {
        Objet ob;
        ob.index = i;
        ob.utility = data.c[i] / (double)data.a[i];
        utilities[i] = ob;
    }

    qsort(utilities, data.n, sizeof(Objet), compare); 
    double* solution = calloc(data.n, sizeof(double)); 
    double remaining_capacity = data.b; 
    double quantity = 0;

    
    for (int j = 0; j < data.n; ++j) {
        if (remaining_capacity == 0) {
            break; 
        }
            double min_quantity = (double)remaining_capacity / data.a[utilities[j].index];
            //printf  ("%f, %d\n", min_quantity, data.a[utilities[j].index]);
            if (min_quantity >= 1) {
                quantity = 1;
            } 
            else {
                quantity = min_quantity ; }
                solution[utilities[j].index] = quantity;
                remaining_capacity -= quantity * data.a[utilities[j].index];
    }

    free(utilities); 
    return solution; 
}
void knapsack(dataSet data) {
    int opt[data.n + 1][data.b + 1];
    bool select[data.n + 1][data.b + 1];
    int i, w;

    for (i = 0; i <= data.n; i++) {
        for (w = 0; w <= data.b; w++) {
            if (i == 0 || w == 0) {
                opt[i][w] = 0;
                select[i][w] = false;
            } else if (data.a[i - 1] <= w && data.c[i - 1] + opt[i - 1][w - data.a[i - 1]] > opt[i - 1][w]) {
                opt[i][w] = data.c[i - 1] + opt[i - 1][w - data.a[i - 1]];
                select[i][w] = true;
            } else {
                opt[i][w] = opt[i - 1][w];
                select[i][w] = false;
            }
        }

    }

    printf("Objets séléctionnés :\n");
    w = data.b;
    int poids_total = 0;
    for (i = data.n; i > 0 && w > 0; i--) {
        if (select[i][w]) {
            printf("\nObjet %d (poids: %d, valeur: %d)\n", i - 1, data.a[i - 1], data.c[i - 1]);
            w -= data.a[i - 1];
            poids_total += data.a[i-1]; 

        }

    }
    printf("Poids des objets sélectionnés: %d\n", poids_total);

    printf("Valeur optimale de la programmation dynamique : %d\n", opt[data.n][data.b]);    
}

void knapsack_preprocessing(dataSet data) {
    int* greedy_solution = KP_greedy(&data);
    double* LP_solution = KP_LP(data);

    int greedy_total_value = 0;
    for (int i = 0; i < data.n; ++i) {
        if (greedy_solution[i] == 1) {
            greedy_total_value += data.c[i];
        }
    }
    printf("Valeur totale des objets pris par l'algorithme glouton : %d\n\n", greedy_total_value);
    int p = -1 ;
    double Lp_total_value = 0;
    for (int i = 0; i < data.n; i++) {
        if (LP_solution[i] > 0) {
            printf("%.2f\n",LP_solution[i]);
            Lp_total_value += data.c[i]*LP_solution[i];
            p = i;
            //printf ("%d\n",p);
        }
    }
    //printf("%d",p) ;
    printf("Valeur totale des objets pris par l'algorithme relaxation linéaire : %.2f\n\n", Lp_total_value);

    printf("\n");

    free(greedy_solution);
    free(LP_solution);

    Objet* obj = malloc(sizeof(Objet) * data.n);
    for (int i = 0; i < data.n; i++) {
        obj[i].index = i;
        obj[i].utility = data.c[i] / (double)data.a[i];
    }
    qsort(obj, data.n, sizeof(Objet), compare);
    
    int* x = malloc(sizeof(int*)*data.n);
    for (int i=0; i< data.n ; i++){
        x[i]= -1 ;
    } 
    
    int reduced_items_number = data.n;
    int reduced_capacity = data.b;

    for (int j = 0; j < data.n; j++) {
        double cj = fabs(data.c[j] - ((double)data.c[p] * data.a[j]) / data.a[p]);
        //printf("%f\n",cj);
        if (cj >= Lp_total_value - greedy_total_value) {
            if (j <=  p - 1) {
                x[j] = 1;
                reduced_items_number -= 1;
                reduced_capacity -= data.a[j];
            } else {
                x[j] = 0;
                reduced_items_number -- ;
            }
        
        }
    }
    
    dataSet reduced_knapsack;
    
    reduced_knapsack.n = reduced_items_number;
    reduced_knapsack.b = reduced_capacity;
    reduced_knapsack.c = (int *)malloc (sizeof(int*)*reduced_items_number);
    reduced_knapsack.a =(int *)malloc(sizeof(int*)*reduced_items_number);

    int j = 0;
    for (int i = 0; i < data.n; ++i) {
        if (x[i] == -1) {
            reduced_knapsack.c[j] = data.c[i];
            reduced_knapsack.a[j] = data.a[i];
            j++;
        }
    }
    
    knapsack(data);

    printf("Selected items after preprocessing:\n");
    for (int i = 0; i < data.n; ++i) {
            printf(" %d", x[i]);
    }

    knapsack(reduced_knapsack);

    const char* filename = "instances_reduites.csv";

    writeSelectedObjectsToCSV(data, x, data.n, filename);

    printf("\n Les objets sélectionnés ont été écrits dans le fichier : %s\n", filename);

    free(obj);
    free(x);
}
