#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

struct graph_s {
    int** adj;
    int n_v;
};

/**
 * La funzione alloca una matrice quadrata, e inizializza tutti i valori a init
 */ 
static int** malloc2dSQ(int n, int init) {
    int** m = (int**)malloc(sizeof(int*)*n);
    for(int i=0; i<n; i++) {
        m[i] = (int*)malloc(sizeof(int)*n);
        for(int j=0; j<n; j++)
            m[i][j] = init;
    }
    return m;
}

graph_t graph_init(int n_v) {
    graph_t g = (graph_t)malloc(sizeof(struct graph_s));
    g->adj = malloc2dSQ(n_v, -1);
    g->n_v = n_v;
    return g;
}

void graph_addEdge(graph_t g, edge_t e) {
    g->adj[e.v][e.w] = e.wt;
}

void graph_removeEdge(graph_t g, edge_t e) {
    g->adj[e.v][e.w] = -1;
}

int graph_vertexAmount(graph_t g) {
    return g->n_v;
}

/**
 * La funzione visita il grafo in profondità.
 * In base ai parametri (diversi da NULL) passati, la funzione può
 * - determinare se è presente un arco backward all'interno del grafo
 * - ricavare l'ordine topologico della funzione
 * Return:
 * - 1 se è stato trovato un arco backward
 * - 0 se non è stato trovato
 */ 
static int dfsR(graph_t g, edge_t e, int* pre, int* post, int* ts, int* ts_i) {
    pre[e.w] = 1;
    for(int v=0; v<g->n_v; v++)
        if(g->adj[e.w][v] != -1)
            if(pre[v] == -1) {
                if(dfsR(g, edge_create(e.w, v, g->adj[e.w][v]), pre, post, ts, ts_i))
                    return 1;
            } else if(post != NULL && post[v] == -1)//L'arco è backward
                return 1;
    if(ts != NULL) /* Se il vettore dell'ordine topologico è valido, aggiungi il vertice appena terminato alla cella libera più a destra */
        ts[--(*ts_i)] = e.w;
    if(post != NULL)
        post[e.w] = 1;
    return 0;
}

/**
 * La funzione determina se il grafo passato per parametro è aciclico.
 * Per far ciò viene effettuata una visita in profondità e viene controllato se è presente un arco backward: se si allora il grafo è ciclico, altrimenti è aciclico.
 * Return:
 * - 1 se il grafo è aciclico
 * - 0 se il grafo è ciclico
 */ 
int graph_isAcyclic(graph_t g) {
    int pre[g->n_v], post[g->n_v];
    int time = 0;
    for(int i=0; i<g->n_v; i++)
        pre[i] = post[i] = -1;

    for(int v=0; v<g->n_v; v++)
        /* Se nella visita in profondità viene trovato un arco backward, allora il grafo non è aciclico */
        if(pre[v] == -1 && dfsR(g, edge_create(v, v, 0), pre, post, NULL, NULL))
            return 0;
    return 1;
}

/**
 * La funzione stampa i cammini massimi da ogni nodo sorgente verso ogni nodo
 */ 
void graph_maxPaths(graph_t g, st_t sym_table) {
    int pre[g->n_v], ts[g->n_v], time, ts_i = g->n_v; //Variabili per ricavo ordine topologico
    int w, st[g->n_v], max_wt[g->n_v], start; //Variabili per ricavo cammino massimo
    int v,i,j; //Variabli ciclo

    /* Ricavo ordine topologico */
    for(v=0; v<g->n_v; v++)
        pre[v] = ts[v] = -1;
    for(v=0; v<g->n_v; v++)
        if(pre[v] == -1)
            dfsR(g, edge_create(v, v, 0), pre, NULL, ts, &ts_i);

    /* Cicla tutti i vertici (vertici di partenza) */
    for(v=0; v<g->n_v; v++) {
        time=0;

        /* Inizializza pesi massimi a 0 e ricava l'indice del nodo di partenza nell'ordine topologico */
        for(i=0; i<g->n_v; i++) {
            st[i] = i;
            max_wt[i] = 0;
            if(ts[i]==v)
                start = i;
        }

        /* Cicla tutti i vertici in ordine topologico (a partire dal nodo di partenza) */
        for(i=start; i<g->n_v; i++) {
            w=ts[i];
            /* Visita riga w nella matrice di adiacenza */
            for(j=0; j<g->n_v; j++)
                /* Applica relaxation inversa sull'arco w-j */
                if(g->adj[w][j]!=-1 && g->adj[w][j] + max_wt[w] > max_wt[j]) {
                    max_wt[j] = g->adj[w][j] + max_wt[w];
                    st[j]=w;
                }
        }

        /* Stampa dati ottenuti */
        printf("Vertice di partenza [%s]:\n", st_getIdByIndex(sym_table, v));
        for(i=0; i<g->n_v; i++) {
            printf("Vertice [%s]: ", st_getIdByIndex(sym_table, i));
            if(i!=v && st[i] == i)
                printf("non raggiungibile\n");
            else
                printf("peso massimo %d, nodo sorgente [%s]\n", max_wt[i], st_getIdByIndex(sym_table,st[i]));
        }
        printf("\n");         printf("Vertice [%s]: ", st_getIdByIndex(sym_table, i));
            if(i!=v && st[i] == i)
                printf("non raggiungibile\n");
            else
                printf("peso massimo %d, nodo sorgente [%s]\n", max_wt[i], st_getIdByIndex(sym_table,st[i]));
        }
        printf("\n");
    }
}

static void free2d(int** m, int n) {
    for(int i=0; i<n; i++)
        free(m[i]);
    free(m);
}

void graph_free(graph_t g) {
    free2d(g->adj, g->n_v);
    free(g);
}