#ifndef SYMBOLTABLE_DEF
#define SYMBOLTABLE_DEF

#include "key.h"

typedef struct st_s* st_t;

st_t st_init();
void st_free(st_t st);

void st_insert(st_t st, keys_t key);

int st_searchByKey(st_t st, keys_t key);
keys_t st_getKey(st_t st, int idx);

void st_print(st_t st);

#endif