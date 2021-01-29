#include "ST.h"
#include <string.h>
#include <stdlib.h>

#define MAX_STANZA 20

struct st_s {
    char** keys;
    int n_keys;
    int max_keys;
};

static char** malloc2d(int r, int c) {
    char** m = (char**)malloc(sizeof(char*)*r);
    for(int i=0; i<r; i++)
        m[i] = (char*)malloc(sizeof(char)*c);
    return m;
}

ST STinit(int max_keys) {
    ST st = (ST)malloc(sizeof(struct st_s));
    st->keys = malloc2d(max_keys, MAX_STANZA+1);
    st->max_keys = max_keys;
    st->n_keys = 0;
    return st;
}

void STadd(ST st, char* key) {
    if(st->n_keys >= st->max_keys)
        return;
    strcpy(st->keys[st->n_keys++], key);
}

int STgetIndex(ST st, char* key) {
    for(int i=0; i<st->n_keys; i++)
        if(strcmp(st->keys[i], key) == 0)
            return i;
    return -1;
}

void STgetKey(ST st, int idx, char* key) {
    if(idx<0 || idx>=st->n_keys)
        strcpy(key, "null");
    else
        strcpy(key, st->keys[idx]);
}

static void free2d(char** m, int r) {
    for(int i=0; i<r; i++)
        free(m[i]);
    free(m);
}

void STfree(ST st) {
    free2d(st->keys, st->n_keys);
    free(st);
}