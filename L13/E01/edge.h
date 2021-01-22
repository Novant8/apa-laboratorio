#ifndef EDGE_H
#define EDGE_H

#include "st.h"

typedef struct {
    int v;
    int w;
    int wt;
} edge_t;

edge_t edge_create(int u, int v, int wt);

void edge_print(edge_t e, st_t st);

#endif