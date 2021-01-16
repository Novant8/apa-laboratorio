#ifndef QUOTAZIONE_H
#define QUOTAZIONE_H

#include "dataora.h"

typedef struct quot_s {
    data_t data;
    float somma;
    int num;
} quot_t;

typedef struct trans_s {
    data_t data;
    ora_t ora;
    float val;
    int num;
} trans_t;

trans_t transazione_leggi(FILE* fp);
int transazione_cmp(trans_t t1, trans_t t2);
trans_t transazione_null();

void quotazione_print(FILE* fp, quot_t q);


#endif