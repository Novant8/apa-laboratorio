#include <stdlib.h>

#include "lista_titoli.h"

typedef struct node_s {
    titolo_t t;
    struct node_s* next;
} *node_t;

struct lista_titoli_s {
    node_t head;
    int n_t; //numero titoli
};

static node_t new_node(titolo_t t, node_t next) {
    node_t n = (node_t)malloc(sizeof(struct node_s));
    n->t = t;
    return n;
}

lista_titoli_t lista_titoli_init() {
    lista_titoli_t l = (lista_titoli_t)malloc(sizeof(struct lista_titoli_s));
    l->n_t = 0;
    return l;
}

void lista_titoli_aggiungi(lista_titoli_t l, titolo_t t) {
    if(l->head == NULL || titolo_cmp(t, l->head->t) < 0) {
        l->head = new_node(t, l->head);
    } else {
        node_t n,p;
        int cmp_res;
        for(p=l->head, n=p->next; n!=NULL && (cmp_res = titolo_cmp(t, n->t)) > 0; p=n, n=n->next);
        p->next = new_node(t, n);
    }
    l->n_t++;
}

/**
 * La funzione restituisce il titolo avente codice passato per parametro all'interno della lista.
 * Se il titolo non è stato trovato, la funzione ritorna un puntatore NULL.
 */ 
static titolo_t cerca_lista(node_t head, char* cod) {
    if(head == NULL)
        return NULL;
    if(titolo_hasCod(head->t, cod))
        return head->t;
    return cerca_lista(head->next, cod);
}

/**
 * Funzione wrapper che va a cercare il titolo avente codice cod passato per parametro all'interno della lista l
 */ 
titolo_t lista_titoli_cerca(lista_titoli_t l, char* cod) {
    return cerca_lista(l->head, cod);
}

void lista_titoli_free(lista_titoli_t l) {
    if(l->head == NULL)
        return;
    node_t n = l->head;
    l->head = n->next;
    lista_titoli_free(l);
    titolo_free(n->t);
    free(n);
}