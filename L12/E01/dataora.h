#ifndef DATAORA_H
#define DATAORA_H

#include <stdio.h>

typedef struct data_s {
    int g;
    int m;
    int a;
} data_t;

typedef struct ora_s {
    int h;
    int m;
} ora_t;

/* Lettura da file */
data_t data_read(FILE* fp);
ora_t ora_read(FILE* fp);

/* Confronto */
int data_cmp(data_t d1, data_t d2);
int ora_cmp(ora_t h1, ora_t o2);

/* Stampa su file */
void data_print(FILE* fp, data_t d);
void ora_print(FILE* fp, ora_t h);

/* Generazione elementi nulli */
data_t data_null();
ora_t ora_null();

/* Controllo se gli elementi sono nulli */
int data_isnull(data_t d);
int ora_isnull(ora_t h);

#endif