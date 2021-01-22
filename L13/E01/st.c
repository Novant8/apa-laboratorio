#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "st.h"

typedef struct item_s {
    char id[MAX_KEY+1];
} item_t;

struct st_s {
    item_t* items;
    int n_items;
    int max_size;
};

st_t st_init(int max_size) {
    st_t st = (st_t)malloc(sizeof(struct st_s));
    st->items = (item_t*)malloc(sizeof(item_t)*max_size);
    st->n_items = 0;
    st->max_size = max_size;
    return st;
}

int st_add(st_t st, char* id) {
    /* Controllo che non si tenti di aggiungere un simbolo ad una tabella piena */
    if(st->n_items >= st->max_size) {
        printf("Tabella di simboli piena!\n");
        return 0;
    }
    strcpy(st->items[st->n_items++].id, id);
    return 1;
}

int st_getIndexById(st_t st, char* id) {
    for(int i=0; i<st->n_items; i++)
        if(strcmp(st->items[i].id, id) == 0)
            return i;
    return -1;
}

char* st_getIdByIndex(st_t st, int i) {
    if(i<0 || i>=st->n_items)
        return NULL;
    return st->items[i].id;
}

void st_free(st_t st) {
    free(st->items);
    free(st);
}