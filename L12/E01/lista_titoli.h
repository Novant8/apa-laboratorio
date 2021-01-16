#ifndef LISTA_TITOLI_H
#define LISTA_TITOLI_H

#include "titolo_azionario.h"

typedef struct lista_titoli_s *lista_titoli_t;

lista_titoli_t lista_titoli_init();
void lista_titoli_free(lista_titoli_t l);

void lista_titoli_aggiungi(lista_titoli_t l, titolo_t t);
titolo_t lista_titoli_cerca(lista_titoli_t l, char* cod);

#endif