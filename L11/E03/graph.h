#ifndef GRAPH_DEF
#define GRAPH_DEF

#include <stdio.h>
#include "st.h"

typedef struct graph_s* graph_t;

/* Inizializzazione e free */
graph_t graph_init();
void graph_free(graph_t g);

/* Carica dati da file */
void graph_load(graph_t g, FILE* fp);

/* Inserimento arco */
void graph_insertEdge(graph_t g, int u, int v, int wt);

/* Generazione lista di adiacenza */
void graph_genLadj(graph_t g);

/* Verifiche di adiancenza tra due vertici */
int graph_isadj_mat(graph_t g, st_t st, int u, int v);
int graph_isadj_list(graph_t g, st_t st, int u, int v);

/* Stampa grafo */
void graph_display(graph_t g, st_t st);

#endif