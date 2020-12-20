#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgList.h"

typedef struct pgNode_s {
    pg_t pg;
    struct pgNode_s* next;
} pgNode_t, *pgLink_t;

struct pgList_s {
    pgLink_t head;
    pgLink_t tail;
    int n_pg;
};

pgList_t pgList_init() {
    pgList_t l = (pgList_t)malloc(sizeof(struct pgList_s));
    l->head = NULL;
    l->tail = NULL;
    l->n_pg = 0;
}

void pgList_read(FILE *fp, pgList_t pgList) {
    pg_t pg;
    while(pg_read(fp, &pg)) {
        pgList_insert(pgList, pg);
        pgList->n_pg++;
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    for(pgLink_t n=pgList->head; n != NULL; n=n->next)
        pg_print(fp, &n->pg, invArray);
}

static pgLink_t pgNode_new(pg_t pg, pgLink_t next) {
    pgLink_t n = (pgLink_t)malloc(sizeof(pgNode_t));
    n->pg = pg;
    n->pg.equip = equipArray_init();
    n->next = next;
    return n;
}

/* IUNSERIMENTO IN CODA */
void pgList_insert(pgList_t pgList, pg_t pg) {
    pgLink_t n = pgNode_new(pg, NULL);
    if(pgList->head == NULL)
        pgList->head = pgList->tail = n;
    else
        pgList->tail = pgList->tail->next = n;
}

void pgList_remove(pgList_t pgList, char* cod) {
    for(pgLink_t n = pgList->head, p; n!=NULL; p=n, n=n->next)
        if(strcmp(n->pg.cod,cod) == 0) {
            if(n == pgList->head)
                pgList->head = n->next;
            else
                p->next = n->next;
            free(n);
            break;
        }
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod) {
    for(pgLink_t n = pgList->head; n!=NULL; n=n->next)
        if(strcmp(n->pg.cod,cod) == 0)
            return &n->pg;
    return NULL;
}

void pgList_free(pgList_t pgList) {
    if(pgList == NULL)
        return;
    if(pgList->head == NULL)
        free(pgList);
    pgLink_t n = pgList->head;
    pgList->head = pgList->head->next;
    free(n);
    pgList_free(pgList);
}