#ifndef TITOLO_AZIONARIO_H
#define TITOLO_AZIONARIO_H

#include <stdio.h>
#include "bst_quotazioni.h"

#define MAX_TITOLO_LEN 20

typedef struct titolo_s *titolo_t;

titolo_t titolo_init();
void titolo_free(titolo_t t);

void titolo_print(FILE* fp, titolo_t t);

void titolo_aggiungi_transazione(titolo_t t, trans_t tr);

quot_t* titolo_getQuot(titolo_t t, data_t d);

/* Ricerca quota massima in un intervallo di date */
quot_t* titolo_minQuot_date(titolo_t t, data_t d1, data_t d2);
/* Ricerca quota massima tra tutte le quote */
quot_t* titolo_minQuot(titolo_t t);

int titolo_cmp(titolo_t t1, titolo_t t2);
int titolo_hasCod(titolo_t t, char* cod);

float titolo_rapportoMaxMin(titolo_t t);
void titolo_bilanciaQuotazioni(titolo_t t);


#endif