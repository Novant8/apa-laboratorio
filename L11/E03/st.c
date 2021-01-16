#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "st.h"

struct st_s {
    int max_n;
    int n;
    keys_t* keys;
};

/**
 * La funzione alloca una matrice di caratteri di dimensioni rxc
 */
static keys_t* key_malloc(keys_t* m, int l) {
    //Se m è nullo allora occorre allocare un nuovo puntatore, altrimenti si rialloca quello passato
    if(m==NULL)
        m = (keys_t*)malloc(sizeof(keys_t)*l);
    else
        m = (keys_t*)realloc(m, sizeof(keys_t)*l);
    return m;
}

st_t st_init() {
    st_t st = (st_t)malloc(sizeof(struct st_s));
    st->keys = key_malloc(NULL, 1);
    st->max_n = 1;
    st->n = 0;
}

/**
 * La funzione inserisce in modo ordinato un vertice nella tabella di simboli
 */ 
void st_insert(st_t st, keys_t key) {
    if(st == NULL)
        return;
    //La dimensione del vettore contenente le chiavi aumenta esponenzialmente in base 2 all'aumentare dei dati inseriti (il numero di vertici è sconosciuto)
    if(st->max_n >= st->n) {
        st->max_n *= 2;
        st->keys = key_malloc(st->keys, st->max_n);
    }
    //Inserimento ordinato della chiave mediante insertion sort
    int i;
    for(i=st->n-1; i>=0 && key_cmp(st->keys[i], key) > 0; i--)
        st->keys[i+1] = st->keys[i];
    st->keys[i+1] = key;
    st->n++;
}

/**
 * La funzione restituisce l'indice del vertice identificato dalla chiave passata per parametro.
 * Se la chiave non viene trovata, la funzione restituisce -1.
 */ 
int st_searchByKey(st_t st, keys_t key) {
    for(int i=0; i<st->n; i++)
        if(key_cmp(st->keys[i], key) == 0) 
            return i;
    return -1;
}

/**
 * La funzione restituisce la chiave del vertice all'indice passato per parametro.
 * Se l'indice non è valido, la funzione restituisce NULL.
 */
keys_t st_getKey(st_t st, int idx) {
    if(idx<0 || idx >= st->n)
        return key_null();
    return st->keys[idx];
}

void st_free(st_t st) {
    free(st->keys);
    free(st);
}