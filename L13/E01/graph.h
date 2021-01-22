#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"

typedef struct graph_s *graph_t;

/* INIT e FREE */
graph_t graph_init(int n_v);
void graph_free(graph_t g);

/* Visita in profondità */
int graph_isAcyclic(graph_t g);

/* Aggiunta arco */
void graph_addEdge(graph_t g, edge_t e);
void graph_removeEdge(graph_t g, edge_t e);

/* Numero vertici */
int graph_vertexAmount(graph_t g);

/* Cammmini massimi */
void graph_maxPaths(graph_t g, st_t sym_table);


#endif