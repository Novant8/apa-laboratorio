#include <string.h>
#include <stdlib.h>

#include "titolo_azionario.h"

struct titolo_s {
    char cod[MAX_TITOLO_LEN+1];
    bst_quot_t quotazioni;
};

titolo_t titolo_init(char cod[]) {
    titolo_t t = (titolo_t)malloc(sizeof(struct titolo_s));
    strcpy(t->cod, cod);
    t->quotazioni = bst_quot_init();
    return t;
}

/**
 * La funzione aggiunge i dati relativi ad una transazione, immagazzinata in un oggetto di tipo quotazione
 */ 
void titolo_aggiungi_transazione(titolo_t t, trans_t tr) {
    bst_quot_aggiungi(t->quotazioni, tr);
}

quot_t* titolo_getQuot(titolo_t t, data_t d) {
    return bst_quot_cerca(t->quotazioni, d);
}

quot_t* titolo_minQuot_date(titolo_t t, data_t d1, data_t d2) {
    return bst_min_quot(t->quotazioni, d1, d2);
}

quot_t* titolo_minQuot(titolo_t t) {
    return titolo_minQuot_date(t, bst_quot_minData(t->quotazioni), bst_quot_maxData(t->quotazioni));
}

void titolo_print(FILE* fp, titolo_t t) {
    printf("Codice %s\nQuotazioni:\n", t->cod);
    bst_quot_print(fp, t->quotazioni);
}

int titolo_cmp(titolo_t t1, titolo_t t2) {
    return strcmp(t1->cod, t2->cod);
}

int titolo_hasCod(titolo_t t, char* cod) {
    return strcmp(t->cod, cod) == 0;
}

float titolo_rapportoMaxMin(titolo_t t) {
    return bst_quot_rapportoMaxMin(t->quotazioni);
}

void titolo_bilanciaQuotazioni(titolo_t t) {
    bst_quot_bilancia(t->quotazioni);
}

void titolo_free(titolo_t t) {
    bst_quot_free(t->quotazioni);
    free(t);
}