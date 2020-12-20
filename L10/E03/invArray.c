#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invArray.h"

struct invArray_s {
    inv_t* items;
    int n_items;
};

invArray_t invArray_init() {
    invArray_t invArray = (invArray_t)malloc(sizeof(struct invArray_s));
    invArray->items = NULL;
    invArray->n_items = 0;
    return invArray;
}

void invArray_read(FILE *fp, invArray_t invArray) {
    fscanf(fp, "%d", &invArray->n_items);
    invArray->items = (inv_t*)malloc(sizeof(inv_t)*invArray->n_items);
    for(int i=0; i<invArray->n_items; i++)
        inv_read(fp, &invArray->items[i]);
}

void invArray_print(FILE *fp, invArray_t invArray) {
    for(int i=0; i<invArray->n_items; i++)
        inv_print(fp, invArray->items + i);
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_print(fp, invArray_getByIndex(invArray, index));
}

inv_t *invArray_getByIndex(invArray_t invArray, int index) {
    return invArray->items+index;
}

int invArray_searchByName(invArray_t invArray, char *name) {
    for(int i=0; i<invArray->n_items; i++)
        if(strcmp(invArray->items[i].nome, name) == 0)
            return i;
    return -1;
}

void invArray_free(invArray_t invArray) {
    free(invArray->items);
    free(invArray);
}