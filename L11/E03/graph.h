#ifndef GRAPH_DEF
#define GRAPH_DEF

#include <stdio.h>
#include "st.h"

typedef struct graph_s* graph_t;

graph_t graph_init();
void graph_free(graph_t g);
void graph_load(graph_t g, FILE* fp);
void graph_insertEdge(graph_t g, int u, int v, int wt);

void graph_genLadj(graph_t g);

int graph_isadj_mat(graph_t g, st_t st, int u, int v);
int graph_isadj_list(graph_t g, st_t st, int u, int v);

void graph_display(graph_t g, st_t st);

#endif