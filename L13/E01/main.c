#include <stdio.h>

#include "st.h"
#include "graph.h"
#include "edge.h"

int fill_st(st_t st, int n_v, FILE* fp);
void fill_graph(graph_t g, st_t st, FILE* fp, edge_t* edges, int* n_e);
void makeDAG(graph_t g, st_t st, edge_t* edges, int n_e);

int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("USO CORRETTO: %s <nome_file>\n", *argv);
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    int n_v;
    if(fp==NULL) {
        printf("Errore in lettura file.\n");
        return -2;
    }
    fscanf(fp, "%d", &n_v);

    st_t st = st_init(n_v);
    if(!fill_st(st, n_v, fp)) {
        printf("Errore nell'inizializzazione della tabella di simboli.\n");
        return -3;
    }

    int n_e;
    edge_t edges[n_v*n_v];
    graph_t g = graph_init(n_v);
    fill_graph(g, st, fp, edges, &n_e);

    fclose(fp);

    if(graph_isAcyclic(g))
        printf("Il grafo è già aciclico, si vanno a calcolare i cammini massimi...\n");
    else
        makeDAG(g, st, edges, n_e);
    printf("\n");

    /* Ricavo cammini massimi */
    graph_maxPaths(g, st);

    /* Liberazione strutture dati */
    st_free(st);
    graph_free(g);

    return 0;
}

/**
 * La funzione riempie una tabella di simboli (già inizializzata), leggendo i suoi dati da file.
 */
int fill_st(st_t st, int n_v, FILE* fp) {
    char id[MAX_KEY+1];
    
    for(int i=0; i<n_v; i++)
        if(!fscanf(fp, "%s", id) || !st_add(st, id))
            return 0;
    return 1;
}

/**
 * La funzione legge da file gli archi del grafo g e lo inizializza.
 * Questa riempie inoltre il vettore di archi letti da file e imposta il parametro n_e passato per referenza pari al numero.
 */ 
void fill_graph(graph_t g, st_t st, FILE* fp, edge_t* edges, int* n_e) {
    char v[MAX_KEY+1], w[MAX_KEY+1];
    int wt;
    *n_e = 0;
    edge_t e;
    while(fscanf(fp, "%s %s %d", v, w, &wt) == 3) {
        edges[*n_e]=edge_create(st_getIndexById(st, v), st_getIndexById(st, w), wt);
        graph_addEdge(g, edges[*n_e]);
        (*n_e)++;
    }
}

/**
 * La funzione, mediante modello delle combinazioni semplici:
 * - Stampa tutti gli insiemi di archi aventi cardinalità car e tali che la loro rimozione va a formare un DAG
 * - Di quelli stampati, trova quello avente peso complessivo massimo e salva i suoi archi in max_wt_e
 * - Restituisce il numero di insiemi trovati
 */ 
int combSempl(int pos, graph_t g, st_t st, int* sol, edge_t* edges, int n_e, edge_t* max_wt_e, int car, int* max_wt, int start, int count) {
    int i;
    if(pos >= car)  {
        int wt=0,    //wt: peso totale dell'insieme
            found=0; //found: indica se l'insieme corrente è valido (la rimozione degli archi trasforma il grafo in un DAG)

        /* Rimozione degli archi dal grafo e calcolo del peso complessivo */
        for(i=0; i<car; i++) {
            graph_removeEdge(g, edges[sol[i]]);
            wt+=edges[sol[i]].wt;
        }

        /* Controllo se il grafo è aciclico: in caso positivo, allora è stato trovato un insieme avente cardinalità pari a car */
        if(graph_isAcyclic(g)) {
            found = 1;

            /* Stampa archi appartenenti all'insieme */
            printf("Trovato insieme di archi avente cardinalità %d:\n", car);
            for(i=0; i<car; i++)
                edge_print(edges[sol[i]], st);
            printf("\n");

            /* Controllo se il peso di tutti gli archi trovati è massimo */
            if(wt > *max_wt) {
                *max_wt = wt;
                for(i=0; i<car; i++)
                    max_wt_e[i] = edges[sol[i]];
            }
        }

        /* Riaggiunta degli archi nel grafo */
        for(i=0; i<car; i++)
            graph_addEdge(g, edges[sol[i]]);
        
        /* In base a se il grafo risultante è aciclico, il conto viene incrementato o no */
        return count+found;
    }
    for(i=start; i<n_e; i++) {
        sol[pos] = i;
        count = combSempl(pos+1, g, st, sol, edges, n_e, max_wt_e, car, max_wt, i+1, count);
    }
    return count;
}

/**
 * La funzione ricava gli insiemi di archi aventi cardinalità minima e salva quello avente peso complessivo maggiore.
 * Gli archi contenuti in quell'insieme vengono rimossi dal grafo g, trasformandolo in un DAG
 */ 
void makeDAG(graph_t g, st_t st, edge_t* edges, int n_e) {
    int n_v = graph_vertexAmount(g);
    int sol[n_e];
    int max_car;
    int max_wt = 0;
    edge_t max_wt_e[n_v];

    /* Si aumenta la cardinalità finchè non viene trovato almeno un insieme in combSempl() */
    for(max_car=1; max_car<n_e && combSempl(0, g, st, sol, edges, n_e, max_wt_e, max_car, &max_wt, 0, 0) <= 0; max_car++);
    
    /* Ciclo di stampa dell'insieme a massimo peso e rimozione degli archi dal grafo, ottenendo un DAG */
    printf("\nInsieme di cardinalità minima (%d) avente peso massimo (%d):\n", max_car, max_wt);
    for(int i=0; i<max_car; i++) {
        edge_print(max_wt_e[i], st);
        graph_removeEdge(g, max_wt_e[i]);
    }
}