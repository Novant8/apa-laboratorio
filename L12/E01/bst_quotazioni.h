#ifndef BST_QUOTAZIONI_H
#define BST_QUOTAZIONI_h

#include "quotazione.h"

typedef struct bst_quot_s *bst_quot_t;

bst_quot_t bst_quot_init();
void bst_quot_free(bst_quot_t bst);

void bst_quot_aggiungi(bst_quot_t bst, trans_t q);

/* Cerca quotazione in una certa data */
quot_t* bst_quot_cerca(bst_quot_t bst, data_t d);

void bst_quot_print(FILE* fp, bst_quot_t bst);

/* Ricerca quotazione minima in un intervallo di date */
quot_t* bst_min_quot(bst_quot_t bst, data_t d1, data_t d2);

/* Ricerca data minima e massima */
data_t bst_quot_minData(bst_quot_t bst);
data_t bst_quot_maxData(bst_quot_t bst);

void bst_quot_bilancia(bst_quot_t bst);

float bst_quot_rapportoMaxMin(bst_quot_t bst);

#endif