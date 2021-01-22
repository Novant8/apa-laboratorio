#include <stdio.h>
#include "edge.h"

edge_t edge_create(int v, int w, int wt) {
    edge_t e;
    e.v = v;
    e.w = w;
    e.wt = wt;
    return e;
}

void edge_print(edge_t e, st_t st) {
    printf("Arco [%s] -> [%s], peso = %d\n", st_getIdByIndex(st, e.v), st_getIdByIndex(st,  e.w), e.wt);
}