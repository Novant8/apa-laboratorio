#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key.h"

int key_cmp(keys_t k1, keys_t k2) {
    int res = strcmp(k1.net, k2.net);
    if(res != 0)
        return res;
    return strcmp(k1.elab, k2.elab);
}

/**
 * La funzione genera una chiave nulla
 * TODO
 */ 
keys_t key_null() {
    keys_t k;
    strcpy(k.elab, "null");
    strcpy(k.net, "null");
    return k;
}

int key_isnull(keys_t k) {
    return strcmp(k.net, "null") == 0 && strcmp(k.elab, "null") == 0;
}

void key_print(keys_t k) {
    printf("RETE [%s], ELAB [%s]\n", k.net, k.elab);
}

keys_t key_read() {
    keys_t k;
    printf("Inserire nome rete: ");
    scanf("%s", k.net);
    printf("Inserire nome elaboratore: ");
    scanf("%s", k.elab);
    return k;
}