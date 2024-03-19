#include <stdio.h>
#include <stdbool.h>
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


/**void generate_random_items(Objet* objets, int n) {
    srand(time(NULL));

    for (int i = 0; i < n; ++i) {
        objets[i].valeurs = rand() % 100 + 1; 
        objets[i].poids = rand() % 50 + 1; 
    }
}*/
#define MAX_ITEMS 100  
#define MAX_WEIGHT 1000

void generate_instance(FILE* file) {
    int n = rand() % MAX_ITEMS + 1;
    int capacity = rand() % MAX_WEIGHT + 1;

    fprintf(file, "%d,%d\n", n, capacity); 

    for (int i = 0; i < n; i++) {
        int poids = rand() % 100 + 1; 
        int valeurs = rand() % 100 + 1;  
        fprintf(file, "%d,%d\n",poids, valeurs); 
    }
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
        //printf("%d\n" , select[i][w]);

    }
    

    printf("Objets séléctionnés:\n");
    w = data.b;
    int poids_total = 0;
    for (i = data.n; i > 0 && w > 0; i--) {
        if (select[i][w]) {
            printf("Objet %d (poids: %d, valeur: %d)\n", i - 1, data.a[i - 1], data.c[i - 1]);
            w -= data.a[i - 1];
            poids_total += data.a[i-1]; 

        }

    }
    printf("Poids des objets séléctionnés: %d\n", poids_total);
    printf("Valeur optimale de la programmation dynamique : %d\n", opt[data.n][data.b]);    
    

}

int compare(const void* a, const void* b) {
    Objets* objA = (Objets*)a;
    Objets* objB = (Objets*)b;
    if (objB->utility - objA->utility > 0) return 1;
    if (objB->utility - objA->utility < 0) return -1;
    return 0;
}
void knapsack_tri(dataSet data) {
    int opt[data.n + 1][data.b + 1];
    bool select[data.n + 1][data.b + 1];
    int i, w;
    
    Objets* utilities = malloc(sizeof(Objets) * data.n);


    for (int i = 0; i < data.n; ++i) {
        Objets ob;
        ob.index = i;
        ob.utility = data.c[i] / (double)data.a[i];
        utilities[i] = ob;
    }

    qsort(utilities, data.n, sizeof(Objets), compare);

    /*for (int i = 0; i <= data.n; i++) {
        printf("Objet %d (poids: %d, valeur: %d, ratio: %f)\n", utilities[i].index, data.a[utilities[i].index], data.c[utilities[i].index], utilities[i].utility);
    }*/
    
    for (i = 0; i <= data.n; i++) {
        for (w = 0; w <= data.b; w++) {
            if (i == 0 || w == 0) {
                opt[i][w] = 0;
                select[i][w] = false;
            } else if (data.a[utilities[i - 1].index] <= w && data.c[utilities[i - 1].index] + opt[i - 1][w - data.a[utilities[i - 1].index]] > opt[i - 1][w]) {
                opt[i][w] = data.c[utilities[i - 1].index] + opt[i - 1][w - data.a[utilities[i - 1].index]];
                select[i][w] = true;
            } else {
                opt[i][w] = opt[i - 1][w];
                select[i][w] = false;
            }
        }
    }
    printf("Objets sélectionnés:\n");
    w = data.b;
    int poids_total = 0;

    for (i = data.n; i > 0 &&  w >0 ; i--) {
        if (select[i][w]) {
            printf("Objet %d (poids: %d, valeur: %d)\n", utilities[i - 1].index, data.a[utilities[i - 1].index], data.c[utilities[i - 1].index]);
            w -= data.a[utilities[i - 1].index];
            poids_total += data.a[utilities[i - 1].index];
        }
    }
    printf("Poids des objets sélectionnés: %d\n", poids_total);
    printf("Valeur optimale de la programmation dynamique : %d\n", opt[data.n][data.b]);    
    
    free(utilities);
}