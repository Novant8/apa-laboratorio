#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "st.h"

typedef struct node_s  {
    int idx;
    int wt;
    struct node_s* next;
} *node_t;

typedef struct list_s {
    node_t head;
    node_t tail;
    int n_el;
} list_t;

struct graph_s {
    int n_v; //num vertici
    int n_e; //num archi
    int** m_adj; //matrice di adiacenza
    list_t* l_adj;
};

/**
 * La funzione alloca una matrice quadrata di interi avente dimensioni nxn e imposta ogni valore di questa a val
 */
static int** madj_init(int n) {
    int** m = (int**)malloc(sizeof(int*)*n);
    for(int i=0; i<n; i++) {
        m[i]=(int*)malloc(sizeof(int)*n);
        for(int j=0; j<n; j++)
            m[i][j] = i==j ? 0 : -1;
    }
    return m;
}

static node_t list_newnode(int idx, int wt, node_t next) {
    node_t n = (node_t)malloc(sizeof(struct node_s));
    n->idx = idx;
    n->wt = wt;
    n->next = next;
    return n;
}

/**
 * La funzione aggiunge un nodo in coda alla lista
 */ 
static void list_add(list_t* l, node_t n) {
    if(l->head == NULL)
        l->head = n;
    else
        l->tail->next = n;
    l->tail = n;
    l->n_el++;
}

static node_t list_search(node_t head, int idx) {
    if(head==NULL || head->idx == idx)
        return head;
    return list_search(head->next, idx);
}

static void free_list(node_t head) {
    if(head == NULL)
        return;
    free_list(head->next);
    free(head);
}

static void free_ladj(list_t* l, int n_v) {
    for(int i=0; i<n_v; i++)
        free_list(l[i].head);
    free(l);
}


static void free_madj(int** m, int n) {
    for(int i=0; i<n; i++)
        free(m[i]);
    free(m);
}

graph_t graph_init(int n_v) {
    graph_t g = (graph_t)malloc(sizeof(struct graph_s));
    g->n_v = n_v;
    g->n_e = 0;
    g->m_adj = madj_init(n_v);
    g->l_adj = NULL;
    return g;
}

void graph_insertEdge(graph_t g, int u, int v, int wt) {
    if(g==NULL)
        return;
    //Non aggiungere un arco se uno dei due indici supera il massimo numero di vertici
    if(u>=g->n_v || v>=g->n_v)
        return;
    g->m_adj[u][v] = g->m_adj[v][u] = wt;
    g->n_e++;
}

void graph_display(graph_t g, st_t st) {
    if(g==NULL)
        return;
    for(int i=0; i<g->n_v; i++) {
        printf("Vertice:\n");
        key_print(st_getKey(st, i));
        printf("\nAdiacenze:\n");
        for(int j=0; j<g->n_v; j++) {
            if(i!=j && g->m_adj[i][j]>=0) {
                key_print(st_getKey(st, j));
                printf(" (flusso: %d)\n", g->m_adj[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * La funzione genera la lista di adiadenza a partire dalla matrice di adiacenza.
 */ 
void graph_genLadj(graph_t g) {
    if(g==NULL)
        return;
    if(g->l_adj != NULL)
        free_ladj(g->l_adj, g->n_v);
    //Alloca vettore di liste lungo n_v (numero di vertici)
    g->l_adj = (list_t*)malloc(sizeof(list_t)*g->n_v);
    for(int i=0; i<g->n_v; i++) {
        g->l_adj[i].head = NULL;
        g->l_adj[i].n_el = 0;
        for(int j=0; j<g->n_v; j++) {
            if(i!=j && g->m_adj[i][j]>=0)
                list_add(g->l_adj+i, list_newnode(j, g->m_adj[i][j], NULL));
        }
    }
}

/**
 * La funzione verifica se due vertici (identificati dalle chiavi passate per parametro) sono adiacenti mediante la matrice di adiacenza
 * Return:
 * - 1 se i vertici sono adiacenti
 * - 0 se i vertici non sono adiacenti, oppure se uno dei due vertici non è valido
 */ 
int graph_isadj_mat(graph_t g, st_t st, int u, int v) {
    if(u<0 || v<0)
        return 0;
    return g->m_adj[u][v]>=0;
}

int graph_isadj_list(graph_t g, st_t st, int u, int v) {
    if(u<0 || v<0)
        return 0;
    return list_search(g->l_adj[u].head, v) != NULL;
}

void graph_free(graph_t g) {
    if(g==NULL)
        return;
    //Libera matrice di adiacenza
    free_madj(g->m_adj, g->n_v);
    //Libera lista di adiacenza se è stata generata
    if(g->l_adj != NULL)
        free_ladj(g->l_adj, g->n_v);
    free(g);
}